/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust Aug 15 cause Peregrins finger_d is badly in need of re-working
// due to changes and I want something working to finish the gwiz package.

#include <security.h>
#include <mudlib.h>

inherit M_ACCESS;


/* pass a USER_OB or a userid */
private nomask string get_level(mixed m)
{
    return adminp(m) ? "admin" : wizardp(m) ? "wizard" : "player";
}

mixed * get_raw_data(string who)
{
    string * info;
    mixed * last;
    object user = find_user(who);

//### also fetch title?
    info = unguarded(1, (: call_other, USER_D, "query_variable",
			 who, ({ "real_name", "email",
#ifdef EVERYONE_HAS_A_PLAN
				     "plan",
#endif
				     }) :));

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

string show_big_finger()
{
    object * u = users();

    return implode(u,
		   (: $1 + sprintf("%-15s%-12s%-5s%s\n",
				   capitalize($2->query_userid()),
				   get_level($2),
				   get_idle($2),
				   query_ip_name($2)) :),
		   sprintf("\n[%s] %d user%s presently connected (%s)\n%79'-'s\n",
			   mud_name(),
			   sizeof(u),
			   sizeof(u) == 1 ? "" : "s",
			   ctime(time()),
			   ""));
}

string get_finger(string who)
{
    object user;
    string * info;
    string mailstring;
    string retval;
    mixed last;
    string email;
    string real_name;
    object mbox;
    int mcount;
    int ucount;

    who = lower_case(trim_spaces(who));

    if ( who == "" )
	return show_big_finger();

    user = find_user(who);

//### also fetch title
    info = unguarded(1, (: call_other, USER_D, "query_variable",
			 who, ({ "real_name", "email",
#ifdef EVERYONE_HAS_A_PLAN
				     "plan",
#endif
				     }) :));
    if ( !info )
    {
//### maybe return 0?
	return "No such player.\n";
    }

    real_name = info[0];
    email = info[1];
    if ( !real_name || real_name == "" )
	real_name = "(none given)";
    if ( !email || email == "" )
	email = "(none given)";

    if ( who != this_user()->query_userid() && !check_privilege(1) )
    {
	if ( email[0] == '#' )
	    email = "(private)";
	if ( real_name[0] == '#' )
	    real_name = "(private)";
    }

    mbox = MAIL_D->get_mailbox(who);
    mcount = mbox->query_message_count();
    ucount = mbox->query_unread_count();
    if ( !mcount )
	mailstring = capitalize(who)+" has no mail.";
    else
    {
	mailstring = sprintf("%s has %d messages.", capitalize(who), mcount);
	if ( ucount )
//	    mailstring[<1..] = sprintf(", %d of which %s unread.",
//				       ucount, ucount > 1 ? "are" : "is");
	    mailstring = (mailstring[0..<2] +
			  sprintf(", %d of which %s unread.",
				  ucount, ucount > 1 ? "are" : "is"));
    }

    last = LAST_LOGIN_D->query_last(who);

//### add in title, linkdeath indication, and idle time

    retval = sprintf("%s\nIn real life: %-36sLevel: %s\n"
		     "%s %s from %s\n%s\nEmail Address: %s\n",
		     capitalize(who),
		     real_name,
		     get_level(who),
		     user ? "On since" : "Last on",
		     last ? ctime(last[0]) : "<unknown>",
		     last ? last[1] : "<unknown>",
		     mailstring,
		     email);

    if ( is_file(WIZ_DIR + "/" + who + "/.plan") )
	retval += "Plan:\n" + read_file(WIZ_DIR + "/" + who + "/.plan");
#ifdef EVERYONE_HAS_A_PLAN
    else if ( info[2] )
	retval += "Plan:\n" + info[2] + "\n";
    else
	retval += "No plan.\n";
#endif

    return retval;
}
