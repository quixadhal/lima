/* Do not remove the headers from this file! see /USAGE for more info. */

#include <daemons.h>


string* quests;
int	points;
int 	deaths;

void save_me();

int
add_quest( string quest, int pts )
{
  if( !quests )
	quests = ({});

  if( previous_object() != find_object( QUEST_D ) )
	return 0;
  if( member_array( quest, quests ) != -1 )
	return 0;

  quests += ({ quest });
  points += pts;
  save_me();
  return 1;
}

int
check_quest( string quest )
{
  return member_array(quest, quests) != -1;
}

string*
query_quests()
{
  return quests;
}


static
void
rack_up_a_death()
{
  deaths++;
  points -= 20;
  save_me();

#ifdef USE_STATUS_LINE
#endif
if( this_body()->has_status_line())  this_body()->update_status_line();
}

int 
query_deaths()
{
  return deaths;
}


int
query_score()
{
  return points;
}
