/* Do not remove the headers from this file! see /USAGE for more info. */

#include <ports.h>

private string lobotomize (string s)
{
  s = lower_case (s);
  return s;
  //  s = replace_string (s, "[","\\[");
  //  return replace_string (s, "]","\\]");
}

private string get_mudlib (string s)
{
  switch (s[0..3])
    {
    case "Lima":
      return "<a href=http://lima.mudlib.org>Lima</a>";
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

private string mail_link (string s)
{
  if (s == "unknown")
    {
      return s;
    }

  return sprintf ("<a href=mailto:%s>%s</a>", s, s);
}

string main (string pattern) 
{
  string	ret = "";
  mapping	mudlist	= IMUD_D->query_mudlist ();
  mapping	upmuds;
  int		nummuds = sizeof (mudlist);
  string	mudname;
  mixed array	data;

  if (!pattern)
    {
      pattern = "*";
    }
  
  pattern = lower_case (pattern);
  pattern = "^" + M_GLOB->translate (pattern, 1);
  mudlist = filter_mapping (mudlist, (: regexp (lobotomize($1), $(pattern)) :));
  if (!sizeof (mudlist))
  {
    return sprintf ("<h3>No muds out of %d match your pattern.<h3>", nummuds);
  }

  if (sizeof (mudlist) != 1)
  {
    ret += "<h3>Which of the following muds are you looking for?</h3>"
      "<br><font size=+1><em>";
    foreach (mudname in sort_array (keys (mudlist), 1))
    {
      ret += sprintf ("<a href=http://%s:%d/cgi/mudinfo.c?=%s>%s</a><br>",
		      __HOST__, PORT_HTTP, mudname, mudname);
    }
    return ret + "</em></font>";
  }
  foreach (mudname, data in mudlist)  // there's only 1
    {
      ret += sprintf ("<TABLE BORDER=5><caption><h2><a href="
		      "telnet://%s:%d>%s</a></h2></caption>"
		      "<TR><TH>Type</TH><TD>%s</TD></TR>"
		      "<TR><TH>Current Mudlib</TH><TD>%s</TD></TR>"
		      "<TR><TH>Base Mudlib</TH><TD>%s</TD></TR>"
		      "<TR><TH>Server</TH><TD>%s</TD></TR>"
		      "<TR><TH>I3 Status</TH><TD>%s</TD></TR>"
		      "<TR><TH>Playability</TH><TD>%s</TD></TR>"
		      "<TR><TH>Admin email</TH><TD>%s</TD></TR>"
		      "</TABLE>",
		      data[1], data[2], mudname, data[8], data[5],
		      get_mudlib (data[6]), data[7], 
		      data[0] == -1 ? "Connected" : "Not connected",
		      data[9], mail_link (data[10]));
    }
  return ret;
}
