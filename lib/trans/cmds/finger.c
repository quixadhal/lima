/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <mudlib.h>
#include <commands.h>

inherit CMD;
inherit M_COMPLETE;
inherit M_GRAMMAR;
inherit CLASS_FINGER;

private nomask string get_idle(int i)
{
    if ( i > 3600 )
	return " (idle " + (i/3600) + "h)";
    if ( i > 60 )
	return " (idle " + (i/60) + "m)";

    return "";
}

private void main( string who )
{
    string mud, name;
    string* matches;
    class finger info;
    string s;
    string first_line;
    string idle;
    string mailstring;
  
    if(!who) who = "";
    who = lower_case(trim_spaces(who));
    if ( sscanf(who, "%s@%s", name, mud) == 2 || sscanf(who, "@%s", mud) )
    {
	string test;

	matches = case_insensitive_complete(M_GLOB->translate(mud),
					    IMUD_D->query_up_muds());

	switch ( sizeof(matches) )
	{
	case 0:
	    outf("No matching mud currently up.\n");
	    return;

	case 1:
	    mud = matches[0];
	    break;

	default:
	    mud = lower_case(mud);
	    foreach ( test in matches )
		if ( lower_case(test) == mud )
		{
		    matches = 0;
		    break;
		}
	    if ( !matches )
		break;
	    outf("Ambigous.  The following muds might apply: %s\n",
		   implode(matches, ", "));
	    return;
	}

	if ( name == "" )
	    IMUD_D->do_who(mud);
	else
	    IMUD_D->do_finger(mud, name);
	return;
    }

    if ( who == "" )
    {
	out(FINGER_D->show_big_finger());
	return;
    }

    info = FINGER_D->get_finger_data(who);
    if ( !info )
    {
	out("No such player.\n");
	return;
    }

    if ( !info->real_name )
	info->real_name = "(private or none given)";
    if ( !info->email )
	info->email = "(private or none given)";
    if ( !info->nickname )
	info->nickname = "(none)";

#ifdef USE_WIZ_POSITION
    if ( !info->position )
	info->position = "(none)";
    info->position = " Position: " + info->position;
#else
    info->position = "";
#endif

    if ( info->home_page )
	info->home_page = "Homepage: " + info->home_page + "\n";
    else
	info->home_page = "";

    if ( info->connect_from )
	info->connect_from = " from " + info->connect_from;
    else
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
	"%s\n"
	"Nickname: %-29s Level: %s\n"
	"In real life: %-25s%s\n"
	"%s %s%s%s\n"
	"%s\n"
	"Email Address: %s\n"
	"%s",
	first_line,
	info->nickname, info->level,
	info->real_name, info->position,
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

    out(s);
}

nomask int valid_resend(string ob)
{
    return ob == CMD_OB_FINGER;
}



