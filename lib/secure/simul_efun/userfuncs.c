/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

string base_name(mixed val);

nomask object this_body()
{
    object u = this_user();

    return u ? u->query_body() : 0;
}

private nomask int filter_users(string str, int lv, object who)
{
    object body = who->query_body();
    mixed *nicks;

    if ( base_name(who) != USER_OB ) return 0;
    if ( body && body->query_hidden() > lv ) return 0;
    if ( who->query_name() == str ) return 1;

    nicks = body && body->query_nicknames();
    if ( nicks && member_array(str, nicks) != -1) return 1;

    return 0;
}

nomask object find_user(string str)
{
    object *choices;
    int lv;

    if ( this_user() )
	lv = this_body()->query_level();
    else
	lv = 0;
    
    choices = filter_array(users(), (: filter_users, str, lv :));

    if ( sizeof(choices) )
	return choices[0];

    return 0;
}


nomask object find_body(string str)
{
    object u = find_user(str);

    return u ? u->query_body() : 0;
}


int
get_level( object ob )
{
  int		level;

  level = userp(ob) ?
	(ob->query_score())/
	(QUEST_D->total_points()/5) :
	ob->query_level();

  return (level<0 ? 0 : ( level > 5 ? 5 : level ));
}


object* all_users(){
  string* valid;
  int dummy;
  string prev_file;

  valid = ({
      "/std/player",
      ANNOUNCE_D,
      TIMEOUT_D,
      FINGER_D,
      NCHANNEL_D,
  });
  prev_file = file_name(previous_object());
  if(!prev_file) return 0;
  sscanf(prev_file,"%s#%d",prev_file,dummy);
  if (member_array(prev_file,valid) == -1) return 0;
  return efun::users();
}
