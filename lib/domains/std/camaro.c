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
  set_long("This is a very cool, dark red Camaro Z28.");
  set_in_room_desc("There's a Camaro Z28 parked here.");
  set_vehicle_msg("$N $vooze coolness as $n $venter the camaro.\n",
                  "$N $vhop out of the camaro.\n");
  set_departure_msg("$N $vzoom off $o.\n");
  set_arrival_msg("$N $vzoom in and $vscreech to a halt.\n");
}

