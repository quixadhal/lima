/* Do not remove the headers from this file! see /USAGE for more info. */

#include <daemons.h>
#include <ports.h>

inherit M_GRAMMAR;
inherit CLASS_FINGER;

// Strip colour codes
string stripped(string info)
{
	return terminal_colour(info, ANSI_D->query_translations()[1]);
}

private nomask string get_idle(int i)
{
  if ( i > 3600 )
    return " (idle " + (i/3600) + "h)";
  if ( i > 60 )
    return " (idle " + (i/60) + "m)";

  return "";
}

string main(mixed data)
{
  class finger info;
  string s;
  string mailstring;
  string idle;
  string first_line;
  string userid;

  if(stringp(data))
    userid=data;
  if(mapp(data))
    userid=data["name"];
  if(strlen(userid) && userid[0] == '=')
    userid = userid[1..];
  userid = lower_case(trim_spaces(userid));
  if ( userid == "" )
    return FINGER_D->show_big_html_finger();

  info = FINGER_D->get_finger_data(userid);
  if ( !info )
    return sprintf("<h2><em>%s</em>: No such player.\n</h2>", userid);

  if ( !info->real_name )
    info->real_name = "(private or none given)";

  if ( !info->email )
    info->email = "(private or none given)";
  else
    info->email = sprintf("<a href=\"mailto:%s\">%s</a>",
        info->email, info->email);
  if ( !info->nickname )
    info->nickname = "(none)";

#ifdef USE_WIZ_POSITION
  if ( !info->position )
    info->position = "(none)";
  info->position = " Position: " + info->position;
#else
  info->position = "";
#endif

  if ( !info->home_page && is_directory(sprintf(WIZ_DIR "/%s/WWW", userid)) )
    info->home_page = sprintf("http://%s:%d/~%s",
        __HOST__, PORT_HTTP, userid);
  if ( info->home_page )
    info->home_page = sprintf("Homepage: <a href=\"%s\">%s</a>\n",
        info->home_page, info->home_page);
  else
    info->home_page = "";

//  if ( info->connect_from )
//    info->connect_from = " from " + info->connect_from;
//  else
    info->connect_from = "";

  if ( info->idle == -1 )
    idle = "";
  else if ( info->idle == -2 )
    idle = " (linkdead)";
  else
    idle = get_idle(info->idle);

  if ( info->mail_count == 0 )
    mailstring = info->visname + " has no mail.";
  else
  {
    mailstring = sprintf("%s has %s.",
        info->visname,
        number_of(info->mail_count, "message"));

    if ( info->mail_unread )
      mailstring[<1..] = sprintf(", %d of which %s unread.",
          info->mail_unread,
          info->mail_unread > 1 ? "are" : "is");
  }

  first_line = info->title ? info->title : info->visname;

  s = sprintf(
    "<title>Finger info for %s@%s</title>\n"
    "<h2>Finger info for %s@%s</h2>\n"
    "<em><font size=+2>%s</font></em>\n"
    "Nickname: %-29s\n"
    "Level: %s\n"
    "In real life: %-25s\n"
    "%s\n"
    "%s %s%s%s\n"
    "%s\n"
    "Email Address: %s\n"
    "%s",
    info->visname, MUD_NAME,
    info->visname, MUD_NAME,
    stripped(first_line),
    info->nickname,
    info->level,
    info->real_name,
    info->position,
    info->idle == -1 ? "Left at" : "On since",
    info->last_login ? ctime(info->last_login) : "<unknown>",
    idle,
    info->connect_from,
    mailstring,
    info->email,
    info->home_page);

  if ( info->plan )
    s += "Plan:\n" + info->plan + "\n";
#ifdef EVERYONE_HAS_A_PLAN
  else
    s += "No plan.\n";
#endif

  return replace_string(s, "\n", "<br>\n");
}
