/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** camaro.c
**
** Example of a vehicle
**
** 951123, Deathblade: Created.
*/

inherit VEHICLE;

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
    set_vehicle_msg("$N $vooze coolness as $n $venter the camaro.\n",
                    "$N $vhop out of the camaro.\n");
    set_arrival_msg("The $N $vzoom off $o.\n");
    set_departure_msg("A $N $vzoom in and $vscreech to a halt.\n");
}


// People can't get at stuff inside the Camaro unless they're in it.
int inventory_accessible()
{


  if(environment(this_body()) != this_object())
    return 0;

  return 1;
}
