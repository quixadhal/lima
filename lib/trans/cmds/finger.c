/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

#include <mudlib.h>
#include <commands.h>

inherit CMD;
inherit M_COMPLETE;

private void main( string who )
{
    string mud, name;
    string* matches;
  
    if(!who) who = "";
    if ( sscanf(who, "%s@%s", name, mud) == 2 || sscanf(who, "@%s", mud) )
    {
	string test;

	matches = case_insensitive_complete(M_GLOB->translate(mud),
					    IMUD_D->query_up_muds());

	switch ( sizeof(matches) )
	{
	case 0:
	    printf("No matching mud currently up.\n");
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
	    printf("Ambigous.  The following muds might apply: %s\n",
		   implode(matches, ", "));
	    return;
	}

	if ( name == "" )
	    IMUD_D->do_who(mud);
	else
	    IMUD_D->do_finger(mud, name);
	return;
    }

    write(FINGER_D->get_finger(who));
}

nomask int valid_resend(string ob)
{
    return ob == CMD_OB_FINGER;
}
