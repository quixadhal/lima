/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** 25-Jul-96    Valentino.     Created.
*/

//:COMMAND
//syntax: people [flags]
//
//Shows a list of players and associated info, with the flags determining
//which players and info are included
//
//Instead of calling the cmd with flags you can do:
//'set people_flags [flags]' the flags that you appoint will be automatically
// used as your default arguments when the command is called.
//
//ex:
//'set people_flags hpnI'
//or
//'people hpnI'
//
//The available flags are as follows.
//--------------------------------------------------------------------------
//W -This flag will show wizards only. It will not print anything, it just
//   filters the users and removes anyone without wizard status.
//P -This flag shows players only. Another filter, this command just removes
//   any wizards from the who list.
//A -This flag shows admins (demigods or Gods) only.
//D -This flag adds an informative line of info triggered by other flags, fex.
//   if your command supports the 'W' flag the info line would print:
//   'Wizards only'
//   -----------------------------------------~
//   <list of wizards>
//   -----------------------------------------~
//
//   above the list of all the wizards logged on.
//h -Huge header. Lists mudname and time on a header 3 lines wide. Automatically
//   appended at the top of the list.
//H -Small header. List mudname and time on a header 1 line wide. Automatically
//   appended to the top of the list.
//l -Adds a line between names on the who list.
//n -Prints the users login name without titles.
//t -Prints the users name and title.
//p -Prints positions, God, Avatar, Demigod, player, ect.
//
//-WIZARD ONLY FLAGS-
//w -Prints brief description of the users enviroment.
//f -Prints filename of the users environment.
//a -Prints either player, wizard or admin titles only.
//i -Prints idle times of people logged on.
//I -Prints IP/hostnames of people logged on.
//F -Prints an 'I' if a person is idle, and an 'E' if the person is editing.

#include <playerflags.h>

inherit CMD;
inherit M_ANSI;

#define WIDTH this_user()->query_screen_width()

#define DELIM (repeat_string("-", WIDTH - 7) + "\n")

#define WHO_FORMAT	"%s:  (Local Time is: %s) %28s\n%s"
#define DEBUG(arg) if (debug && member_array(arg,msgs)==-1) msgs+=({arg})

string *msgs=({});

string get_who_string(string arg)
{
    string retval="";
    int debug, no_delim;
    object array b = bodies() - ({ 0 });
    string *args=({});
    string tmp;

    if (arg)
    {
	args=explode(arg,"");
    }
    else
    {
	tmp=get_user_variable("people_flags");
	if (tmp)
	    args=explode(tmp,"");
	else
	{
	    /*	  write("Syntax: people [Flags]\n"+
		  "      flags: WPADhlntp\n");
	    if (wizardp(this_user()))
	      write("Wizardflags: wfaiIF\n");
	    return "See the manpages for more help.\n";
	    */
	    args = explode( "anFwh", "" );
	}
    }
    if (member_array("D",args)!=-1)
    {
	debug=1;
	args-=({"D"});
    }
    if(member_array("S", args) !=-1)
    {
      no_delim = 1;
      args -= ({"S"});
    }
    if(!no_delim)
        retval+=DELIM;

    if (member_array("W",args)!=-1)
    {
	b=filter_array(b, (: wizardp($1) :));
	args-=({"W"});
	DEBUG("Wizards only");
    }
    else
    if (member_array("A",args)!=-1)
    {
	b=filter_array(b, (: adminp($1) :));
	args-=({"A"});
	DEBUG("Admins only");
    }
    else
    if (member_array("P",args)!=-1)
    {
	b=filter_array(b, (: !wizardp($1) :));
	args-=({"P"});
	DEBUG("Players only");
    }
    if (!wizardp(this_user()))
    {
	b=filter_array(b, (: $1->query_body()->is_visible():) );
    }

    if (member_array("h",args)!=-1)
    {
	args-=({"h"});
	DEBUG("Header");
  retval += colour_center(implode(explode(mud_name(), ""), " ")) + "\n";
  retval += colour_center("(PST is: " + ctime(time()) + ")") + "\n";
  retval += colour_center("There are " + sizeof(b) + " users connected.") + "\n";
  retval += no_delim?"":DELIM;
    }
    else
    if (member_array("H",args)!=-1)
    {
	args-=({"H"});
	DEBUG("Small Header");
	retval += sprintf("%s:  (Local Time is: %s)\n",
	  mud_name(), ctime(time()));
    if(!no_delim)
	    retval+=DELIM;
    }
    foreach (object body in b)
    {
	foreach (string arg2 in args)
	{
	    switch (arg2)
	    {
	    case "l":
		DEBUG("Delimiter");
		retval+=DELIM;
		break;
	    case "n":
		DEBUG("Names");
		retval+=sprintf("%-14s ",body->query_name());
		break;	
	    case "w":
		if (!wizardp(this_user())) break;
		retval+=sprintf("%-25.25s ", environment(body) ?
		  environment(body)->get_brief() : 
		  "(Nowhere)");
		break;
	    case "f":
		if (!wizardp(this_user())) break;
		retval+=sprintf("%-20s ", environment(body) ?
		  file_name(environment(body)) : "(lost?)");
		break;
	    case "t":
		DEBUG("Titles");
		retval+=sprintf("  %s  ",body->query_formatted_desc(78));
		break;	
	    case "p":
		DEBUG("Position");
		retval+=sprintf("%|14.14s ",body->query_wiz_position() ?
		  body->query_wiz_position() : "(None)");
		break;
	    case "a":
		if (!wizardp(this_user())) break;
		DEBUG("Position II");
		if (wizardp(body))
		    retval+=sprintf("%-10s ",adminp(body) ? "Admin" : "Wizard");
		else
		    retval += "Player     ";
		break;
	    case "i":
		if (!wizardp(this_user())) break;
		DEBUG("Idle times");
		retval+=sprintf("%-8.8s ", query_idle(body->query_link()) ? 
		  convert_time(query_idle(body->query_link()),2) :
		  "");
		break;
	    case "I":
		if (!wizardp(this_user())) break;
		DEBUG("Ip's");
		retval+=sprintf("%s ", query_ip_name(body->query_link()));
		break;
	    case "S":
		if (!wizardp(this_user())) break;
		retval += body ? body->query_score() : 0;
		break;
	    case "F":
		if (!wizardp(this_user())) break;
		DEBUG("Flags");
		retval+=sprintf("%2d%2c%c ",
		  body->query_score(),
		  (query_idle(body->query_link()) > 60 ? 'I':' '),
		  (body && body->test_flag(F_IN_EDIT) ? 'E' : ' ')
		);
		break;
	    default:
		if(arg2)
		    retval += "Who: Unknown flag '"+arg2+"' -  ignored.\n";
	    }
	}
	retval+="\n";
    }
  if(!no_delim)
      retval+=DELIM;
    return retval;
}

private void main(string arg)
{
    string outtie;
    if( arg == "" )
	arg = 0;
    msgs=0;
    msgs=({});
    outtie=get_who_string(arg);
    if (sizeof(msgs)) outf(implode(msgs,", ")+".\n");
    out(outtie);
}

void player_menu_entry()
{
    bare_init();
    main(0);
    done_outputing();
}
