/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** vehicle.c
**
** An object that can be driven around.
**
** 951123, Deathblade: created
** 951221, Rust: rewrote it completely due to the new mesaging system.
*/

inherit NON_ROOM;
inherit M_SMARTMOVE;
inherit M_MESSAGES;

private nosave string arrival_msg;
private nosave string departure_msg;
// Something like: sitting, standing, for: sitting on the horse you see...
// This is really a gerundive, but we'll call it a verb so as not to confuse
// people.
private nosave string primary_verb;
private nosave int notify_all = 1;


protected void set_move_notification(int a)
{
    notify_all = a;
}

protected void set_arrival_msg(string s)
{
  arrival_msg = s;
}

protected void set_departure_msg(string s)
{
  departure_msg = s;
}

protected void set_primary_verb(string s)
{
  primary_verb = s;
}

string get_arrival_msg()
{
  return arrival_msg;
}

string get_departure_msg()
{
  return departure_msg;
}

string get_primary_verb()
{
  return primary_verb;
}

//### this isn't right, but I didn't want to redesign _too_ much just yet
string *get_player_message(string message, mixed arg) {
    string mess;

    if ( message == "leave" )
      {
	mess = get_arrival_msg() ||  "The $N $vleave $o.\n";
      }
    else if ( message == "enter" )
      {
	mess = get_departure_msg() || "The $N $venter.\n";
      }

    return action( ({ this_object() }), mess, arg);
}

string query_final_exit_value(string str, int is_normal)
{
  string ret;

  if((!(ret = ::query_exit_value(str, is_normal)) || ret[0] == '#')
     && environment())
    return environment()->query_exit_value(str, is_normal);

  return ret;
}

int can_travel()
{
  return 1;
}

object array get_riders()
{
  return filter(all_inventory(this_object()), (:$1->is_living():));
}

string show_contents()
{
  if(environment(this_body()) != this_object())
    return inventory_recurse(0, get_riders());

  return ::show_contents();
}

string get_riders_as_string()
{
  // Get the inventory of the horse.
  object array inv = all_inventory(this_object());
  // Filter out anything that isn't living.
  object array riders = filter(inv, (: $1->is_living() :));

  if(!sizeof(riders))
    {
      return 0;
    }

  // Return a string that's a comma seperated list of the riders.
  if(sizeof(riders) == 1)
    {
      return riders[0]->short();
    }
  return implode(map(riders[0..<2], (: $1->short() :)), ", ")+ " and " + 
    riders[<1]->short();
}

string query_in_room_desc()
{
  string verb = get_primary_verb();
  string base_desc = ::query_in_room_desc();


  if(!inventory_visible() || !sizeof(get_riders()))
    {
      return base_desc;
    }
  else
    {
      return (base_desc ? base_desc + "\n" : "") + 
		  capitalize((verb ? verb + " " : "") +
			query_prep() + " " + the_short() + " you see " +
			get_riders_as_string()+".");
    }
}

//Override of notify_move() from M_SMARTMOVE so we can tell all
// the passengers that we moved. (if we want to)

void notify_move()
{
    object *inv;
    object curr;
    if(!notify_all)
        return;
    inv = all_inventory(this_object());
    foreach(curr in inv)  {
        if(curr->query_link())
            curr->force_game_command("look");
    }
}

 

int is_vehicle()
{
  return 1;
}
