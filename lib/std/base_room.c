/* Do not remove the headers from this file! see /USAGE for more info. */

//  a room.c
// Rust@ZorkMUD,   Jan 12, 1994
// Nevyn@GUE       1-14-94
// Cleave@ZorkMUD 2-25-94 Added long(), long default.
// Beek@ZorkMud   4-6-94  Added the way cool exit code.  Look at the
//      lfun docs.
// Rust wrote in_room stuff to make looking in rooms more interesting
// Beek & Rust have both done a bit of work on this since....
// August 24 - Beek added light
// Satan@ArcadiaF 10-29-95 Added the OBVIOUS_EXITS_BOTTOM, casue I don't like
//      the way the exits look on the top, and Beek didn't want to fix it.
// Satan@ArcadiaF 10-30-95 Added set_hidden_exits(). This makes a list that
//      of exits that are hidden from all in the room.
//      Wizards can see DARK_EXITS as they are denoted with a *name format.
// 951113, Deathblade: removed some obsolete vars; made them all private

#include <setbit.h>
#include <move.h>
#include <hooks.h>

inherit CONTAINER;
inherit M_ITEMS;
inherit __DIR__ "room/exits";
inherit __DIR__ "room/roomdesc";

private static string area_name;

//:FUNCTION stat_me
//Writes some debugging info about the object.  Shows the container info,
//as well as the short and exits.
int stat_me() {
    printf("Room: %s [ %s ]\n\n",
      short(), implode(query_exit_directions(1), ", "));
    container::stat_me();
    return 1;
}

//:FUNCTION set_brief
//Set the name of the room seen at the top of the description and in brief mode
void set_brief(string str) {
    set_proper_name(str);
}

//:FUNCTION can_hold_water
//Return 1 if the object can hold water.
/* by default, rooms can hold water */
int can_hold_water()
{
    return 1;
}

void create()
{
    container::create();
    exits::create();

    // cloned rooms will already have this stuff happen... We need this
    // because setup() is the way people configure mudlib objects.
    // Almost always, except in the case of rooms, game objects are clones.
    if( !clonep(this_object()) )
    {
	// initialize the mudlib (default) stuff, then the area coder's
	mudlib_setup();
	this_object()->internal_setup();

	setup();
    }
}

void mudlib_setup()
{
    ::mudlib_setup();
    set_light(DEFAULT_LIGHT_LEVEL);
    set_max_capacity(1000000);
    add_id_no_plural("here");
    add_id("environment");
}

//:FUNCTION set_area
//Used by weather to know which rooms are the same as this one.
void set_area(string name)
{
  area_name = name;
}

//:FUNCTION set_area
//Find out what 'area' the room belongs to.  See set_area.
string get_area()
{
    return area_name;
}

string query_name ()
{
    return "the ground";
}

// Conflict resolution
string long()
{
  return roomdesc::long();
}

string get_brief()
{
  return short();
}
