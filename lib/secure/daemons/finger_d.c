/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust Aug 15 1994

#include <security.h>
#include <net/http_d.h>

inherit DAEMON;
inherit M_GRAMMAR;

/* pass a USER_OB or a userid */
private nomask string get_level(mixed m)
{
    return adminp(m) ? "admin" : wizardp(m) ? "wizard" : "player";
}

mixed * get_raw_data(string who)
{
    string * info;
    mixed email;
mixed real_name;
    mixed * last;
    object user = find_user(who);

    //### also fetch title?
    info = unguarded(1, (: call_other, USER_D, "query_variable",
	who, ({ "real_name", "email",
#ifdef EVERYONE_HAS_A_PLAN
	  "plan",
#endif
	}) :));

    email = info[1];
    if(email[0] == '#')
info[1] = 0;

    real_name= info[0];
    if (real_name[0] == '#')
info[0] = 0;
 
    info[1] = "(private)";
    last = LAST_LOGIN_D->query_last(who);
    return ({ capitalize(who),
		  0,
		  info[0],
		  info[1],
		  last ? ctime(last[0]) : "<unknown>",
		  user ? query_idle(user) : -1,
		  last ? last[1] : "<unknown>",
		  get_level(who),
		  sizeof(info) > 2 ? info[2] : 0
		  });
}

private nomask string get_idle(object o)
{
    int i;

    i = query_idle(o);
    if ( i > 3600 )
	return (i/3600) + "h";
    if ( i > 60 )
	return (i/60) + "m";

    return "";
}

string show_big_finger(int htmlize)
{
    object * u = users();
    string retval;

    if(!htmlize)
      return implode(u,
		   (: $1 + sprintf("%-15s%-12s%-5s%s\n",
                       find_body( $2->query_userid()) ? capitalize( $2->query_userid()):"Login",
				   get_level($2),
				   get_idle($2),
				   query_ip_name($2)) :),
		   sprintf("\n[%s] %d user%s presently connected (%s)\n%79'-'s\n",
			   mud_name(),
			   sizeof(u),
			   sizeof(u) == 1 ? "" : "s",
			   ctime(time()),
			   ""));
    else
      {
	retval = sprintf("<META HTTP-EQUIV=\"Refresh\" CONTENT=30><center><TABLE BORDER=5><caption><font size=+2>%s</font></caption>"
			 "<TR><TH>Name</TH><TH>Position</TH><TH>Idle</TH><TH>On From</TH></TR>",
			 mud_name());
	foreach (object user in u)
	  {
	    retval += sprintf("<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>",
			      find_body( user->query_userid()) ? 
		       sprintf("<font size=+2><a href=http://%s:%d/cgi/who.c?=%s>%s</a></font>", 
			       __HOST__, HTTP_PORT, 
			       user->query_userid(), capitalize( user->query_userid())) :"Login",
				   get_level(user),
				   get_idle(user),
				   query_ip_name(user));
	  }

	return retval + "</TABLE></center>";
      }
}

varargs string get_finger(string who, int htmlize)
{
    object user;
    string * info;
    string mailstring;
    string retval;
    mixed last;
    string first_line;
    string email;
    string real_name;
    string nickname;
    string position;
    string url;
    string idle;
    object mbox;
    int mcount;
    int ucount;
    string connect_from;

    who = lower_case(trim_spaces(who));

    if ( who == "" )
	return show_big_finger(htmlize);

    user = find_user(who, 1);	/* find even linkdead users */

    info = unguarded(1, (: call_other, USER_D, "query_variable",
			 who, ({ "real_name",
				     "email",
				     "nickname",
				     "url",
#ifdef USE_WIZ_POSITION
				     "wiz_position",
#endif
#ifdef USE_TITLES
				     "title",
#endif
#ifdef EVERYONE_HAS_A_PLAN
				     "plan",
#endif
				     }) :));
    if ( !info )
    {
//### maybe return 0?
	return htmlize ? sprintf("<h2><em>%s</em>: No such player.\n</h2>",
				 who) : "No such player.\n";
    }

    real_name = info[0];
    email = info[1];
    nickname = info[2];
    url = info[3];
    position = info[4];

    if ( !real_name || real_name == "" )
	real_name = "(none given)";
    if ( !email || email == "" )
	email = "(none given)";
    if( !nickname || nickname == "" )
	nickname = "(none)";

    if ( !position || position == "" )
	position = "(none)";
#ifdef USE_WIZ_POSITION
    position = "Position: " + position;
#else
    position = "";
#endif

    if( !url )
	url = "";

    if ( !this_user() || 
	 (who != this_user()->query_userid() && !check_privilege(1) ))
    {
	if ( email[0] == '#' )
	    email = "(private)";
	if ( real_name[0] == '#' )
	    real_name = "(private)";
	if ( url[0] == '#' )
	    url = "(private)";
    }
    if (htmlize && email[0] != '(')
      {
	email = sprintf("<A href=mailto:%s>%s</A>", email, email);
      }
    if ( url == "" && is_directory (sprintf("/wiz/%s/WWW", who)))
      url = sprintf("http://%s:%d/~%s", __HOST__,HTTP_PORT ,who);
    if ( url != "" )
	url = "Homepage: " + (htmlize ?  (url[0..6] == "http://" ? 
			      sprintf ("<a href=%s>%s</a>", url,url) :
			      sprintf ("<a href=http://%s>http://%s</a>", 
				       url, url)) : url) + "\n";

#ifdef USE_TITLES
# ifdef USE_WIZ_POSITION
    first_line = info[5];
# else
    first_line = info[4];
# endif
    if ( !first_line )
	/* use the following line... */
#endif
    first_line = capitalize(who);

    if (htmlize)
      first_line = "<em><font size=+2>" + first_line + "</font></em>";

    mbox = MAILBOX_D->get_mailbox(who);
    mcount = mbox->query_message_count();
    ucount = mbox->query_unread_count();
    if ( !mcount )
	mailstring = capitalize(who)+" has no mail.";
    else
    {
        mailstring = sprintf("%s has %s.", capitalize(who), number_of( mcount, "message" ));
	if ( ucount )
	    mailstring[<1..] = sprintf(", %d of which %s unread.",
				       ucount, ucount > 1 ? "are" : "is");
    }

    last = LAST_LOGIN_D->query_last(who);

    if ( !user )
	idle = "";
    else if ( !interactive(user) )
	idle = " (linkdead)";
    else
    {
	idle = get_idle(user);
	if ( idle != "" )
	    idle = " (idle " + idle + ")";
    }

    if ( wizardp(this_user()) )
	connect_from = " from " + (last ? last[1] : "<unknown>");
    else
	connect_from = "";

    retval = sprintf("%s\n"
		     "Nickname: %-29s %sLevel: %s\n"
		     "In real life: %-25s %s%s\n"
		     "%s %s%s%s\n%s\nEmail Address: %s\n%s",
		     first_line,
		     nickname,
		     htmlize ? "\n" : "",
		     get_level(who),
		     real_name,
		     htmlize ? "\n" : "",
		     position,
		     user ? "On since" : "Left at",
		     last ? ctime(last[0]) : "<unknown>",
		     idle,
		     connect_from,
		     mailstring,
		     email,
		     url);

    if ( is_file(WIZ_DIR + "/" + who + "/.plan") )
	retval += "Plan:\n" + read_file(WIZ_DIR + "/" + who + "/.plan");
#ifdef EVERYONE_HAS_A_PLAN
    else if ( info[<1] )
	retval += "Plan:\n" + info[<1] + "\n";
    else
	retval += "No plan.\n";
#endif

    return htmlize ? replace_string (retval, "\n", "<br>\n") : retval;
}
