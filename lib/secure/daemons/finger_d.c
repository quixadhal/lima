/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust Aug 15 1994

#include <security.h>
#include <ports.h>

inherit M_ACCESS;
inherit M_GRAMMAR;
inherit CLASS_FINGER;

void create() {
    set_privilege(1);
}

/* pass a USER_OB or a userid */
private nomask string get_level(mixed m)
{
    return adminp(m) ? "admin" : wizardp(m) ? "wizard" : "player";
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

class finger get_finger_data(string userid)
{
    class finger result;
    object user;
    string * info;
    mixed last;
    object mbox;

    info = unguarded(1, (: call_other, USER_D, "query_variable",
			 userid, ({ "real_name",
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
	return 0;

    result = new(class finger,
//### the visname should be queried from somewhere...
		 visname: capitalize(userid),
		 real_name: info[0],
		 email: info[1],
		 nickname: info[2],
		 home_page: info[3],
		 level: get_level(userid)
		 );

    if ( result->real_name == "" )
	result->real_name = 0;
    if ( result->email == "" )
	result->email = 0;
    if ( result->nickname == "" )
	result->nickname = 0;
    if ( result->home_page == "" )
	result->home_page = 0;

#ifdef USE_WIZ_POSITION
    result->position = info[4];
    if ( result->position == "" )
	result->position = 0;
#endif

    if ( !this_user() ||
	 (userid != this_user()->query_userid() && !check_privilege(1) ))
    {
#ifdef PLAYER_INFO_IS_PUBLIC
	if ( result->email && result->email[0] == '#' )
	    result->email = 0;
	if ( result->real_name && result->real_name[0] == '#' )
	    result->real_name = 0;
	if ( result->home_page && result->home_page[0] == '#' )
	    result->home_page = 0;
#else
	result->email =0;
	    result->real_name = 0;
	    result->home_page = 0;
#endif
    }

#ifdef USE_TITLES
# ifdef USE_WIZ_POSITION
    result->title = info[5];
# else
    result->title = info[4];
# endif
#endif

    mbox = MAILBOX_D->get_mailbox(userid);
    result->mail_count = mbox->query_message_count();
    result->mail_unread = mbox->query_unread_count();

    last = LAST_LOGIN_D->query_last(userid);

    user = find_user(userid, 1);	/* find even linkdead users */
    if ( !user )
	result->idle = -1;
    else if ( !interactive(user) )
	result->idle = -2;
    else
	result->idle = query_idle(user);

#ifdef PLAYER_INFO_IS_PUBLIC
//### a person can get this info via I3 as a player... let it thru here
    if ( 1 /* wizardp(this_user()) */ )
#else
    if ( check_privilege(1) )
#endif
	result->connect_from = last ? last[1] : 0;

    result->last_login = last ? last[0] : 0;

    if ( is_file(WIZ_DIR + "/" + userid + "/.plan") )
	result->plan = read_file(WIZ_DIR + "/" + userid + "/.plan");
#ifdef EVERYONE_HAS_A_PLAN
    else if ( info[<1] )
	result->plan = info[<1];
#endif

    return result;
}

string show_big_finger()
{
    object * u = users();

    return implode(u,
		   (: $1 + sprintf("%-15s%-12s%-5s%s\n",
				   find_body( $2->query_userid()) ? capitalize( $2->query_userid()):"Login",
				   get_level($2),
				   get_idle($2),
				   query_ip_name($2)) :),
		   sprintf("\n[%s] %d user%s presently connected (%s)\n%s\n",
			   mud_name(),
			   sizeof(u),
			   sizeof(u) == 1 ? "" : "s",
			   ctime(time()),
			   repeat_string("-", 75)));
}

string show_big_html_finger()
{
    object * u = users();
    string retval;

    retval = sprintf("<META HTTP-EQUIV=\"Refresh\" CONTENT=30><center><TABLE BORDER=5><caption><font size=+2>%s</font></caption>"
		     "<TR><TH>Name</TH><TH>Position</TH><TH>Idle</TH><TH>On From</TH></TR>",
		     mud_name());
    foreach (object user in u)
    {
	retval += sprintf("<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD></TR>",
			  find_body( user->query_userid()) ? 
			  sprintf("<font size=+2><a href=http://%s:%d/cgi/who.c?=%s>%s</a></font>", 
				  __HOST__, PORT_HTTP, 
				  user->query_userid(), capitalize( user->query_userid())) :"Login",
			  get_level(user),
			  get_idle(user),
			  query_ip_name(user));
    }

    return retval + "</TABLE></center>";
}

void clean_up()  {
    destruct(this_object());
}
