/*
** This file is part of the Lima distribution mudlib.  
** Do not remove this header.  Please see /USAGE for more 
** information on using this code.
**
** FILE: /trans/obj/telnet_ob.c
** AUTHOR: John Viega (rust@virginia.edu)
** CREATED: Jul 22 1995
**
** OVERVIEW
** ========
** This object allows people with permissions to open sockets
** the ability to telnet.  It allows for multiple sessions, and
** knows the addresses of muds on the intermud already.
**
** TODO
** =====
** Add telnet negotiations.
**
** REVISIONS
** =========
** Rust. Jul 25 1995 - / isn't the command character if you have the 
**                     telnet_char environment variable set.  Suggested
**                     by Satan@ArcadiaF.
**                   - Added a /close to close the current connection.
**                   - Added /hide to hide all input from any telnet session.
**                     (it toggles.)  Suggested by Zifnab.
*/

#include <mudlib.h>
#include <socket.h>

#define TELNET_HELP_FILE "/help/admin/telnet"
#define IAC  255

string *iac_regex = ({sprintf("%c..",IAC)});
string iac = sprintf("%c",IAC);


inherit M_REGEX;
inherit DAEMON;

class connection
{
  string address;
  object socket;
  string output_buffer;
  string session_name;
  string session_base_name;
}


private static class connection *sessions = ({});
private static int 		active_session = -1;
private static string	 	active_session_name;
private static int 		actively_elsewhere;
private static int 		notify_on_activity = 1;
private static object           owner;
private static string           control_char;
private static int              hide_all_output;


private nomask string get_address(string);
private nomask string get_unique_session_name(string);
private nomask void open_socket(class connection);
private nomask void recv_input_from(string, object, string);
private nomask void close_session(string, object);
private nomask void handle_command(string);
nomask void remove();


private nomask void init_session(string session_name, string address)
{
  class connection new_connection;
  int i;

  new_connection = new(class connection);
  for(i=0;i<sizeof(sessions);i++){
    if (!sessions[i] || !sessions[i]->socket)
      break;
  }

  if(i != sizeof(sessions))
    sessions[i] = new_connection;
  else
    sessions += ({ new_connection });

  printf ("Opening connection #%i...\n", i+1);
  new_connection->address = get_address(address);
  new_connection->session_name = get_unique_session_name(session_name);
  new_connection->session_base_name = session_name;
  new_connection->output_buffer = "";

  if(catch(open_socket(new_connection)))
     {
       write("Session could not be opened.\n");
       return;
     }
  active_session = i;
  active_session_name = new_connection->session_name;
}
  

private nomask string get_address(string potential_mudname)
{
  mapping mudlist;
  string* matches;
  mudlist = IMUD_D->query_mudlist();
  
  matches = insensitive_regexp(keys(mudlist), potential_mudname);
  
  if(sizeof(matches) != 1)
    return potential_mudname;

  return sprintf("%s %i", mudlist[matches[0]][1], mudlist[matches[0]][2]);
}

private nomask string get_unique_session_name(string session_name)
{
  int i = 1;
  class connection con;
  
  foreach(con in sessions){
    if(!con || !stringp(con->session_name))
      continue;
    if(con->session_base_name == session_name)
      i++;
  }

  if(i == 1)
    return session_name;
  
  return sprintf("%s<%i>", session_name, i);
}

private nomask void open_socket(class connection this_con)
{
  string session = this_con->session_name;

  this_con->socket = new(SOCKET, SKT_STYLE_CONNECT,
			 this_con->address,
			 (: recv_input_from, session :),
			 (: close_session, session :));
}


private nomask void recv_input_from(string session, object sock, string input)
{
  string	*info;
  int		i;
  int           session_num = -1;

    for(i=0;i<sizeof(sessions);i++)
      {
        if(sessions[i]->session_name == session)
  	{
  	  session_num = i;
  	}
      }

  //  BBUG(catch(info = reg_assoc(input,iac_regex, ({0}))[0]));
  // tmp hack cause my reg_assoc errors where it shouldn't.
  info = ({input});
  for(i=0;i<sizeof(info);i++)
    {
      if(i%2) {
//	handle_negotiation(info[i]);
	continue;
      }
      if((!hide_all_output) && active_session_name == session)
	{
	  map(explode(info[i],"\n"), 
	  (:tell_object(owner, 
			sprintf("~ %s\n",$1)):));
	  continue;
	}
      else
	{
	  if(notify_on_activity && sessions[session_num]->output_buffer == "")
	    {
	      tell_object(owner, sprintf("%%There is activity in %s\n",
					 session));
	    }
	  sessions[session_num]->output_buffer += input;
	}
    }
}


private nomask void handle_input(mixed input)
{
  if(input == -1)
    {
      remove();
    }
  if(strlen(input) > 1 && (input[0] == control_char[0]))
    {
      handle_command(input[1..]);
      return;
    }

  if(actively_elsewhere)
    {
      class connection con;

      con = sessions[active_session];
      con->socket->send(input + "\n");
      return;
    }
  else
    this_user()->modal_pass(input);
}

void init_telnet()
{
  if(owner) 
    return;
  owner = this_body();
  control_char = this_body()->query_shell_ob()->get_variable("telnet_char") ||
    "/";
  this_user()->modal_push((: handle_input :), "# ");
}

nomask void remove() {
  class connection con;

  foreach(con in sessions)
    {
      if(!con) continue;
      catch(con->socket->remove());
    }
  destruct(this_object());
}


private nomask void handle_command(string cmd)
{
  string arg;
  class connection con;
  int i;

  sscanf(cmd,"%s %s", cmd, arg);
  switch(cmd)
    {
    case "hide" :
      if(!hide_all_output)
	{
	  hide_all_output = 1;
	  write("Hiding all telnet output.\n");
	}
      else
	{
	  hide_all_output = 0;
	  map(explode(sessions[active_session]->output_buffer, "\n"),
	      (: printf("~ %s\n", $1) :));
	  sessions[active_session]->output_buffer = "";
	}
      return;
    case "send" :
      if(active_session == -1)
	{
	  write("Perhaps you should open a session first.\n");
	  return;
	}
      con = sessions[active_session];
      con->socket->send(arg + "\n");
      return;

    case "open" :
      init_session(arg, arg);
      return;

    case "goto" :
      i = to_int(arg);
      if(i<1 || i > sizeof(sessions) || !sessions[i-1])
	{
	  write("Invalid session.\n");
	  return;
	}
      active_session = i-1;
      active_session_name = sessions[active_session]->session_name;
      write("Switching.\n");
      map(explode(sessions[active_session]->output_buffer, "\n"),
	  (: printf("~ %s\n", $1) :));
      sessions[active_session]->output_buffer = "";
      return;
      
    case "jobs" :
      if(!sizeof(filter(sessions, (: typeof($1) == "class" :)))){
	write("You currently have no sessions open.\n");
	return;
      }
      write("Active sessions:\n================\n");
      for(i=0;i<sizeof(sessions);i++){
	if(typeof(sessions[i]) != "class")
	  continue;
	printf("%i:  %s\n", i+1, sessions[i]->session_name);
      }
      return;
    case "exit" :
      this_user()->modal_pop();
      remove();
      return;
    case "tog" :
      if(!actively_elsewhere && active_session == -1)
	{
	  write("But you have no active sessions....\n");
	  return;
	}
      if(actively_elsewhere)
	actively_elsewhere = 0;
      else
	actively_elsewhere = 1;
      write("Ok.\n");
      return;

    case "help" :
    case "?" :
      new(MORE_OB)->more_file(TELNET_HELP_FILE);
      return;

    case "close" :
      close_session(active_session_name, sessions[active_session]->socket);
      return;

    default:
      write("Unknown telnet command.\n");
      return;
    }
}

private nomask void close_session(string ses, object sock)
{
  int i;
  int new_session = -1;

  tell_object(owner, sprintf("%%Session '%s' is closed.\n", ses));

  for(i=0; i<sizeof(sessions); i++)
    {
	if(!sessions[i])
	continue;
      if(sessions[i]->session_name == ses)
	{
	  sessions[i] = 0;
	}
      else if(new_session == -1 && sessions[i])
	{
	  new_session = i;
	}
    }
  active_session = new_session;
  if(active_session == -1)
    {
      write("No more active sessions.\n");
      if(actively_elsewhere)
	{
	  write("Doing a /tog for you.\n");
	  actively_elsewhere = 0;
	}
    }
  else
    {
      write("Use /goto to return to another session.\n");
    }
  catch(sock->remove());
}
