/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** camaro.c
**
** Example of a vehicle
**
** 951123, Deathblade: Created.
*/

inherit VEHICLE;

void create()
{
    object ob;

    if ( !clonep() )
	return;

    ::create();
    set_adj("red");
    set_id("car", "camaro");
    set_long("This is a very cool, dark red Camaro Z28.");
    set_in_room_desc("There's a Camaro Z28 parked here.\n");
    set_inside_destination("/domains/std/camaro_inside", 1);
}
