/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;

mapping quests = ([]);
static int total_points;

#define QUEST_FILE	"/data/daemons/quests"


private
void
calculate_total_points()
{
  int		i;
  string* 	keys;
 
  total_points = 0;

  keys = keys( quests );
  i = sizeof( keys );
  while( i-- )
    total_points += quests[keys[i]][1];
}

int
add_quest( string quest, int value, string base )
{

  
  if(!check_privilege(1)) return 0;
  if(!
  unguarded( 1, (: write_file, "/log/quests",
		 sprintf("%-30s worth %d pts, added by %s.\n", base, value,
			 this_body()->query_name()) :) )
 ) return 0;

  quests[quest] = ({ base , value , 0 });
  calculate_total_points();
  save_object(QUEST_FILE);
  return 1;
}

int
delete_quest( string quest )
{
  if(!check_privilege(1) || !quests[quest])
    return 0;

  map_delete( quests, quest );
  save_object(QUEST_FILE);
  calculate_total_points();
  return 1;
}

int
grant_points( object solver, string quest )
{
  if( base_name( previous_object() ) != quests[quest][0] )
    return 0;
  
  if( !solver->add_quest( quest, quests[quest][1] ) )
    return 0;
  quests[quest][2]++;
  tell_object( solver, sprintf("Your score has gone up by %d points.\n",
	quests[quest][1]) );
  unguarded(1, (: save_object, QUEST_FILE :));
  return 1;
}

string
quest_dump()
{
  string*	keys;
  int		i;
  string	logfile;
  string	output;
  int		total;
  
  logfile = "/log/QUESTS";
  unguarded( 1, (: rm, logfile :) );

  keys = keys( quests );
  keys = sort_array( keys, 1);
 
  i = sizeof( keys );
  output = "";
  while( i-- )
  {
    output += sprintf("%-20s-> %-2d points, %d solves, home: %s\n",
      keys[i], quests[keys[i]][1], quests[keys[i]][2], quests[keys[i]][0]);
    total += quests[keys[i]][1];
  }

  output += sprintf("Total points: %d\n",total);

  unguarded(1, (: write_file, logfile, output :) );
  return output;
}

string
show_quest( string quest )
{
  if( !quests[quest] )
	return sprintf("%s: no such quest milestone.\n", quest);

  return sprintf("%-20s-> %-2d points, %d solves, home: %O",quest, quests[quest][1],
  quests[quest][2], quests[quest][0]);
}

create()
{ 
    ::create();
    restore_object(QUEST_FILE, 1);
    if (!quests)
	quests = ([]);
    calculate_total_points();
}

int
total_points()
{
  return total_points;
}
