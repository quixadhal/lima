/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz

#include <mudlib.h>

inherit CMD;
inherit M_COMPLETE;

private void main( string who )
{
    string mud, name;
    string* matches;
  
  if(!who) who = "";
    if ( sscanf(who, "%s@%s", name, mud) == 2 || sscanf(who, "@%s", mud) )
    {

      matches = case_insensitive_complete(M_GLOB->translate(mud),IMUD_D->query_mudnames());
      switch(sizeof(matches))
	{
	case 0:
	  printf("No such known mud: %s\n", mud);
	  break;
	case 1:
	  mud = matches[0];
	  if ( name == "" )
	    IMUD_D->do_who(mud);
	  else
	    IMUD_D->do_finger(mud, name);
	  break;
	default:
	  printf("Ambigous.  The following muds might apply: %s\n", implode(matches,", "));
	  break;
	}
      return;
    }

    write(FINGER_D->get_finger(who));
}
