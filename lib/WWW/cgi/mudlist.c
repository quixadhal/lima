/* Do not remove the headers from this file! see /USAGE for more info. */

inherit M_GLOB;
inherit M_REGEX;

private string get_mudlib(string s)
{
  switch (s[0..3])
    {
    case "Lima":
      return "<a href=http://lima.mudlib.org:7881>Lima</a>";
    case "Nigh":
      return sprintf("<a href=http://www.imaginary.com/LPC/Nightmare/>%s</a>", s);
    case "Disc":
      return sprintf("<a href=http://www.imaginary.com:5678/>%s</a>", s);
    case "Tmi-":
      return sprintf("<a href=http://130.89.226.53:5550/>%s</a>", s);
    default:
      return s;
    }
}

private string get_driver(string s)
{
  switch (s[0..4])
    {
    case "MudOS":
      return "<a href=http://www.mudos.org>" + s + "</a>";
    default:
      return s;
    }
}

private string mail_link(string s)
{
  if (s == "unknown")
    {
      return s;
    }

  return sprintf("<a href=mailto:%s>%s</a>", s, s);
}

string main(string pattern) 
{
  string	ret = "";
  mapping	mudlist	= IMUD_D->query_mudlist();
  int		nummuds = sizeof(mudlist);
  string array  muds = keys(mudlist);
  array 	data;
  string array  matches;
  array         arg;

  if ( !pattern )
    matches = muds;
  else
    {
      matches = insensitive_regexp(muds, "^" + translate(pattern));
      if ( !sizeof(matches) )
	{
	  return "<h3>No muds match your pattern.</h3>";
 	}
    }
  
  matches = filter_array(matches, (: $(mudlist)[$1][0] == -1 :));
  ret += sprintf("<TABLE BORDER=5><caption><h2>There are currently "+
		  sizeof(matches)+" muds UP out of "+
		  sizeof(mudlist)+ " muds on the I3:"+
		  "</h2></caption>"+
		  "<TR><TH>Mud Name</TH><TH>Mudlib</TH><TH>Server</TH>"+
		  "<TH>Type</TH>"+
		  "<TH>Mud Status</TH><TH>Admin contact</TH></TR>");

   foreach ( string mudname in sort_array(matches, 1) )
    {
      data = mudlist[mudname];
      ret += sprintf("<TR><TD><font size=+2><em>"+
		      "<a href=telnet://%s:%d>%s</a></em>"+
		      "</font></TD><TD>%s</TD><TD>%s</TD><TD>%s"+
		      "</TD><TD>%s</TD>"
		      "<TD>%s</TD></TR>",
		      data[1], 
		      data[2], 
		      mudname, 
		      get_mudlib(data[6]), 
		      get_driver(data[7]), 
		      data[8], 
		      data[9],
		      mail_link(data[10]));
    }
   ret += "</TABLE><p><p>";
   return ret;
}
