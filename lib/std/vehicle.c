/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** vehicle.c
**
** An object that can be driven around.
**
** 951123, Deathblade: created
** 951221, Rust: rewrote it completely due to the new mesaging system.
** 032299, Tigran: Heavy modifications to work with the new exit system
*/

inherit NON_ROOM;
inherit M_SMARTMOVE;
inherit M_MESSAGES;

private nosave int notify_all = 1;
private nosave string entrancemsg;
private nosave string exitmsg;
nosave function set_driving_hook;
nosave function change_driving_hook;

protected void set_move_notification(int a)
{
  notify_all = a;
}

protected void set_arrival_msg(string s)
{
  add_msg("enter",s);
}

protected void set_departure_msg(string s)
{
  add_msg("leave",s);
}


string get_arrival_msg()
{
  return query_msg("enter");
}

string get_departure_msg()
{
  return query_msg("leave");
}


void set_vehicle_msg(string inmsg, string outmsg)
{
    entrancemsg = inmsg;
    exitmsg = outmsg;
}

mixed default_message(int enter)
{
    if (is_player_inside() && enter)
    {
	return exitmsg;
    }
    else if (!is_player_inside() && enter)
    {
	return entrancemsg;
    }
    return "";
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

mixed enter_check()
{
  if(environment(this_body())!=environment(this_object())&&
     query_relation(this_body()) )
    return "You are already there.  Maybe you should stand up first.";
  return 1;
}

void change_driving_hook_func(string direction)
{
  object array new_driver=get_riders();
  this_body()->set_driving_vehicle(0);
  if(sizeof(new_driver))
    {
      new_driver[0]->set_driving_vehicle(1);
      new_driver[0]->simple_action("$N $vmove into the drivers position.",new_driver[0]);
    }
}
  

void set_driving_hook_func(string direction)
{
  if(sizeof(get_riders())==1)
    this_body()->set_driving_vehicle(1);
}


void mudlib_setup()
{
    ::mudlib_setup();
    change_driving_hook=(:change_driving_hook_func:);
    set_driving_hook=(:set_driving_hook_func:);
    /* Add the relations that should more than likely exist on all vehicles.  
     * It seems as though "on" is really the only one that is applicable.
     * If it isn't, well then remove the relation and add the appropriate one*/
    add_relation("in",VERY_LARGE*3);
    set_default_relation("in");
    add_hook("person_arrived",set_driving_hook);
    add_hook("person_left",change_driving_hook);
    add_method("get in",this_object(),(:enter_check:));
    add_method("get out",(:environment(this_object()):));
    set_default_enter_message( (: default_message, 1 :) );
    set_default_exit_message((: default_message, 0 :));
}
