/* Do not remove the headers from this file! see /USAGE for more info. */

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
      return "<a href=http://lima.imaginary.com>Lima</a>";
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
  pattern = M_GLOB->translate (pattern);
  mudlist = filter_mapping (mudlist, (: regexp (lobotomize($1), $(pattern)) :));
  /*  if (!sizeof (mudlist))
    {
      return sprintf ("<h3>No muds out of %d match your pattern.<h3>", nummuds);
    }
    */
  
  upmuds  = filter_mapping (mudlist, (: $2[0] == -1 :));

  if (!sizeof (upmuds))
    {
      return "<h3>No muds match your pattern.</h3>";
    }

  mudlist = filter_mapping (mudlist, (: $2[0] != -1 :));

  if (sizeof (upmuds))
    {
      ret += sprintf ("<TABLE BORDER=5><caption><h2>Currently on the I3:</h2></caption>"
		      "<TR><TH>Mud Name</TH><TH>Mudlib</TH><TH>Server</TH><TH>Type</TH>"
		      "<TH>Mud Status</TH><TH>Admin contact</TH></TR>");
      foreach (mudname in sort_array (keys (upmuds), 1))
	{
	  data = upmuds[mudname];
	  ret += sprintf ("<TR><TD><font size=+2><em><a href=telnet://%s:%d>%s</a></em>"
			  "</font></TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD>"
			  "<TD>%s</TD></TR>",
			  data[1], data[2], mudname, get_mudlib (data[6]), 
			  data[7], data[8], data[9],
			  mail_link (data[10]));
	}
	ret += "</TABLE><p><p>";
    }
  /*  
  if (sizeof (mudlist))
    {
      ret += sprintf ("<TABLE BORDER=5><caption><h2>Not Connected (may be down):<h2></caption>"
		      "<TR><TH>Mud Name</TH><TH>Mudlib</TH><TH>Server</TH><TH>Type</TH>"
		      "<TH>Mud Status</TH><TH>Admin contact</TH></TR>");
      foreach (mudname in sort_array (keys (mudlist), 1))
	{
	  data = mudlist[mudname];
	  ret += sprintf ("<TR><TD><font size=+2><em><a href=telnet://%s:%d>%s</a></em>"
			  "</font></TD><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD>"
			  "<TD><a href=mailto:%s>%s</a></TD></TR>",
			  data[1], data[2], mudname, data[6], data[7], data[8], data[9],
			  data[10], data[10]);
	}
	ret += "</TABLE>";
    }
    */
  return ret;
}
