/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** camaro.c
**
** Example of a vehicle
**
** 951123, Deathblade: Created.
*/

inherit VEHICLE;
inherit M_ENTERABLE;

void setup()
{
    set_adj("red");
    set_id("camaro", "car");
    set_preposition("in");
    set_long("This is a very cool, dark red Camaro Z28.");
    set_max_capacity(VERY_LARGE*4);
    // So people will see: Sitting in the Camaro you see Rust...
    set_primary_verb("sitting");
    set_in_room_desc("There's a Camaro Z28 parked here.");
// Not to be confused w/ set_enter_msg() and set_exit_msg() which
// are part of room exits and which will work if you've got other rooms
// off this one.
    set_on_enter_msg("$N $vooze coolness as $n $venter the camaro.\n");
    set_on_exit_msg("$N $vhop out of the camaro.\n");
    set_arrival_msg("The $N $vzoom off $o.\n");
    set_departure_msg("A $N $vzoom in and $vscreech to a halt.\n");
}

int direct_drive_obj()
{
  return 1;
}

int direct_drive_obj_str()
{
  return 1;
}

// People can't get at stuff inside the Camaro unless they're in it.
int inventory_accessible()
{


  if(environment(this_body()) != this_object())
    return 0;

  return 1;
}
