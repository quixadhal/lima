/* Do not remove the headers from this file! see /USAGE for more info. */

// An NNTP client object.   (RFC 977: Network News Transfer Protocol)
// Protocol by Brian Kantor and Phil Lapsley.
// Created: May 4, 1995 by John Viega (rust@virginia.edu)

#include <socket.h>
#include <mudlib.h>

#define NNTP_PORT "119"
#define MAX_PORT 32000 

DOC_MODULE("interface to an external nntp server.")



private object   news_socket;
private string   news_host;
private string   news_port;
private string*  expected_responses = ({"WELCOME"});
private string*  post_queue = ({});
private function response_callback;
private string*  long_response;
private int      long_response_code;
private int      can_post = 1; // assume you can until we get the WELCOME 
                               // screen.  This SHOULD be known in advance...

private int waiting_for_end_of_long_response = 0;

private string* long_responses = ({ 100, 215, 220, 221, 222, 230, 231});
private string CRLF = "\r\n";


private nomask void handle_response(int code, mixed response)
{
  string this_response = expected_responses[0];
  int count, first, last;
  string name;
  int i;
  mixed id, nr, tmp;

  if(sizeof(expected_responses) > 1)
    expected_responses = expected_responses[1..<1];
  else
    expected_responses = ({});

  switch(this_response)
  {
  case "WELCOME":
    if(code == 200)
      can_post = 1;
    else
      can_post = 0;
    return;
  case "LIST":
    if(code != 215)
      {
	evaluate(response_callback, "nntp error: unexpected reply.");
	return;
      }
    evaluate(response_callback, map_array(response[1..],(:explode($1," "):)));
    return;
  case "GROUP":
    if(code != 211)
      {
	evaluate(response_callback, "nntp error: unexpected reply.");
	return;
      }
    response = explode(response, " ");
    i = sizeof(response);
    if(i>1)
      {
	count = response[1];
	if(i>2)
	  {
	    first = response[2];
	    if(i>3)
	      {
		last = response[3];
		if(i>4)
		  {
		    name = response[4];
		  }
	      }
	  }
      }
    evaluate(response_callback, code, count, first, last, name);
    return;

  case "STAT_NEXT_OR_LAST":
    if((code-22)%100)
      {
	evaluate(response_callback, "nntp error: unexpected reply.");
	return;
      }
    response = explode(response, " ");
    id = "";
    i = sizeof(response);
    if(i > 1)
      {
	nr = response[1];
	if(i > 2)
	    id = response[2];
      }
    evaluate(response_callback, code, nr, id);
    return;
  case "ARTICLE":
    if((code-22)%100)
      {
	evaluate(response_callback, "nntp error: unexpected reply.");
	return;
      }
    tmp = explode(response, " ");
    id = "";
    i = sizeof(tmp);
    if(i > 1)
      {
	nr = tmp[1];
	if(i > 2)
	    id = tmp[2];
      }
    if(sizeof(response) > 1)
      response = response[1..];
    else
      response = ({});
    evaluate(response_callback, nr, id, response);
    return;

  case "XHDR":
    evaluate(response_callback, "nntp error: not implemented yet.");
    return;

  case "QUIT":
    evaluate(response_callback, response);
    news_socket->remove();
    return;
   
  //Urg, don't send IHAVE and something else in the same execution thread. 
  //I'll add a workaround in a day or 2.
  case "IHAVE":
    if(code < 300 || code > 399)
      {
	evaluate(response_callback, "nntp error: unexpected reply.");
	return;
      }
    expected_responses += ({"IHAVE_CONFIRMATION"});
    tmp = post_queue[0];
    if(sizeof(post_queue) > 1)
      post_queue = post_queue[1..];
    else
      post_queue = ({});
    for(i=0;i<sizeof(tmp);i++)
      {
	if(tmp[i] == ".")
	  tmp[i] = "..";
	news_socket->send(tmp[i] + CRLF);
      }
    news_socket->send("."+ CRLF);
    evaluate(response_callback, response);
    return;
    
  default:
    evaluate(response_callback, response);
    return;

  }
    
}  

private nomask void nntp_read_callback(object socket, string data)
{
  int num_lines, i, response_code;
  string *lines_read;
  string response;

  BBUG("Reading: " + data);
  lines_read = explode(data,"\n");
  num_lines = sizeof(lines_read);
  
  for(i=0; i<num_lines ;i++)
  {
    if(waiting_for_end_of_long_response)
      {
	if(trim_spaces(lines_read[i]) == ".")
	  {
	    waiting_for_end_of_long_response = 0;
	    handle_response(long_response_code, long_response);
	    continue;
	  }
	long_response += ({lines_read[i]});
	continue;
      }
    if(sscanf(lines_read[i], "%d %s", response_code, response) != 2)
      {
	evaluate(response_callback, "nntp error: invalid response : "+ lines_read[i]+".");
	continue;
      }
    
    if(member_array(response_code, long_responses) != -1)
      {
	waiting_for_end_of_long_response = 1;
	long_response_code = response_code;
	long_response = ({lines_read[i]});
	continue;
      }
    handle_response(response_code, lines_read[i]);
  }
}

private nomask void nntp_close_callback(object socket)
{
  
}

varargs void create(function callback, string host, mixed port) 
{
    if(!callback && !host && !port)
      return;
    if(!functionp(callback))
      error("bad type arg 1 to create an NNTP object.");
    response_callback = callback;
    if(!host || !stringp(host))
	error("bad type arg 2 to create NNTP.");
    if(!stringp(port))
    {
	if(!port)
	{
	    port = NNTP_PORT;
	} 
	else
	{
	    if(intp(port) && port > 0 && port <= MAX_PORT)
	    {
		port = sprintf("%d", port);
	    }
	    else
	    {
		error("Bad type argument 3 to create an NNTP object.");
	    }
	}
    }
    news_host   = host;
    news_port   = port;
    news_socket = clone_object(SOCKET, SKT_STYLE_CONNECT, host+" "+port,
			       (: nntp_read_callback :), (: nntp_close_callback :));

}


DOC(newgroups, "Send an nntp NEWGROUPS command.")

void newgroups(string date, string time)
{
    expected_responses += ({"NEWGROUPS"});
    news_socket->send("NEWGROUPS "+date+" "+time+CRLF);
}

DOC(newnews, "Send an nntp NEWNEWS command.")

void newnews(string group, string date, string time)
{
    expected_responses += ({"NEWNEWS"});
    news_socket->send("NEWNEWS " + group + " " + date + " " + time+CRLF);
}

DOC(list, "Send an nntp LIST command.")

void list()
{
    expected_responses += ({"LIST"});
    news_socket->send("LIST"+CRLF);
}

DOC(group, "Send an nntp GROUP command.")

void group(string name)
{
    expected_responses += ({"GROUP"});
    news_socket->send("GROUP "+name+CRLF);
}

DOC(help, "Send an nntp HELP command.")

void help()
{
    expected_responses += ({"HELP"});
    news_socket->send("HELP"+CRLF);
}

DOC(stat, "Send an nntp STAT command.")

void stat(string id)
{
    expected_responses += ({"STAT_NEXT_OR_LAST"});
    news_socket->send("STAT " + id+CRLF);
}

DOC(next, "Send an nntp NEXT command.")

void next()
{
    expected_responses += ({"STAT_NEXT_OR_LAST"});
    news_socket->send("NEXT"+CRLF);

}

DOC(last, "Send an nntp LAST command.")

void last()
{
    expected_responses += ({"STAT_NEXT_OR_LAST"});
    news_socket->send("LAST"+CRLF);
}    

DOC(head, "Send an nntp HEAD command.")

varargs void head(string id)
{
    expected_responses += ({"ARTICLE"});
    if(!id)    
	news_socket->send("HEAD"+CRLF);
    else	
        news_socket->send("HEAD "+id+CRLF);
}


DOC(body, "Send an nntp BODY command.")

varargs void body(string id)
{
    expected_responses += ({"ARTICLE"});
    if(!id) 
	news_socket->send("BODY"+CRLF);
    else
    news_socket->send("BODY "+id+CRLF);
}


DOC(article, "Send an nntp ARTICLE command.")

varargs void article(string id)
{
  expected_responses += ({"ARTICLE"});
  if(!id) 
      news_socket->send("ARTICLE"+CRLF);
  else
      news_socket->send("ARTICLE "+id+CRLF);
}

DOC(xhdr, "Send an nntp XHDR command.")

void xhdr(string hdr, string str)
{
  expected_responses += ({"XHDR"});
  news_socket->send("XHDR "+hdr+" "+str+CRLF);
}

DOC(post, "Send an nntp POST command.")

void post(string file)
{
  mixed contents;
  int i;
  if(can_post != 1)
    {
      evaluate(response_callback, "nntp error: not confirmed for posting.");
      return;
    }
  if(catch(contents = explode(read_file(file),"\n")))
    {
      evaluate(response_callback, "nntp error: couldn't find local file.");
      return;
    }
  expected_responses += ({"POST"});
  post_queue += ({file});
  news_socket->send("POST"+CRLF);
  for(;i<sizeof(contents); i++)
    {
      if(contents[i] == ".")
	contents[i] = "..";
      news_socket->send(contents[i]+CRLF);
    }
  news_socket->send("."+CRLF);
  
}

DOC(slave, "Send an nntp SLAVE command.")

void slave()
{
  expected_responses += ({"SLAVE"});
  news_socket->send("SLAVE"+CRLF);
}

DOC(ihave, "Send an nntp IHAVE command.")

void ihave(string id, mixed file)
{
  if(can_post != 1)
    {
      evaluate(response_callback, "nntp error: not confirmed for posting.");
      return;
    }
  if(catch(file = explode(read_file(file),"\n")))
    {
      evaluate(response_callback, "nntp error: couldn't find local file.");
      return;
    }
  expected_responses += ({"IHAVE"});
  post_queue += ({file});
  news_socket->send("IHAVE "+id+CRLF);
}

DOC(quit, "Send an nntp QUIT command, and clean up.")

void quit()
{
  expected_responses += ({"QUIT"});
  news_socket->send("QUIT"+CRLF);
}

void remove()
{
  catch(news_socket->remove());
}
