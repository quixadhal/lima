/* Do not remove the headers from this file! see /USAGE for more info. */

// Belboz

//:PLAYERCOMMAND
//USAGE: 	score
//
//Tells you your score.

#include <daemons.h>

inherit CMD;

private void main()
{
  int		pts,total;
  string	rank;
#ifdef USE_SIMPLE_LEVEL
  int lev = this_body()->query_level();
#endif

#ifdef USE_SIMPLE_EXP
  int exp = this_body()->query_experience();
#endif

  if( wizardp(this_user()) )
  {
    out("You are an implementor.\n");
  }

  pts = this_body()->query_score();
  total = QUEST_D->total_points();
  if(total)
    switch( (100*pts)/total )
    {
      case 0:  rank = "complete and utter newbie";break;
      case 1..5: rank = "rank amateur";break;
      case 6..10: rank = "tyro";break;
      case 11..20: rank = "not quite rank, but still amateur";break;
      case 21..30: rank = "tenderfoot spelunker";break;
      case 31..40: rank = "second-class spelunker";break;
      case 41..50: rank = "first class spelunker";break;
      case 51..60: rank = "adventurer";break;
      case 61..70: rank = "expert adventurer";break;
      case 71..80: rank = "skilled player";break;
      case 81..90: rank = "complete and utter player";break;
      case 91..99: rank = "veteran player";break;
      default:
        if(pts < 0 )
	  rank = "blundering buffoon";
        else
	  rank = "Dungeon Master";
    }
#ifdef USE_SIMPLE_LEVEL
  outf("Your level is %d.\n",lev);
#endif

#ifdef USE_SIMPLE_EXP
  outf("You have %d experience points.\n",exp);
#endif
  if(total)
    outf("You have earned %d out of the %d points on %s.\n"
         "This gives you the rank of %s.\n", pts, total, mud_name(), rank );
  else
    outf("There are no points or anything on %s, so I guess you're doing Ok.\n", mud_name());
}