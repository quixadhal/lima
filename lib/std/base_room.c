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
inherit M_GETTABLE;
inherit M_EXIT;
inherit __DIR__ "room/roomdesc";
inherit __DIR__ "room/state";

private nosave string array area_names = ({ });

//:FUNCTION stat_me
//Returns some debugging info about the object.  Shows the container info,
//as well as the short and exits.
string stat_me()
{
    return sprintf("Room: %s [ %s ]\n\n",
      short(), implode(query_exit_directions(1), ", ")) +
    container::stat_me();
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

void create(array args...)
{
    container::create();

    // cloned rooms will already have this stuff happen... We need this
    // because setup() is the way people configure mudlib objects.
    // Almost always, except in the case of rooms, game objects are clones.

    //### Small problem here; some rooms are clones, and this bails for them.
    //### We really need a better way to test if we should call setup().
    //###
    //### Virtual grids, for example, usually depend on cloned rooms working
    //### properly.
    //###
    //### BTW, contrary to the above, it appears this is *NOT* already done for cloned rooms

#if 0
    if( !clonep() )
    {
#endif
	// initialize the mudlib (default) stuff, then the area coder's
	mudlib_setup();
	this_object()->internal_setup();

	setup(args...);
#if 0
    }
#endif
}

void mudlib_setup()
{
    ::mudlib_setup();
    set_light(DEFAULT_LIGHT_LEVEL);
    /* First add the relation 'in'. */
    add_relation("in",1000000);
    /* Make it the default relation for all rooms */
    set_default_relation("in");
    add_id_no_plural("here");
    add_id("environment");
    set_gettable( -1 );
    set_flag( ATTACHED );
}

//:FUNCTION set_area
//Used by m_wander to prevent monsters from wandering to far.
//Can either be a string, or an array of strings
void set_area(string array names...)
{
    area_names = names;
}

//:FUNCTION query_area
//Find out what 'areas' the room belongs to.  See set_area.
string array query_area()
{
    return area_names;
}

string query_name()
{
    return "the ground";
}

string get_brief()
{
    return short();
}

void possible_light_change(int old_light, int new_light) {
    if (old_light && !new_light) {
	tell_from_inside(this_object(), "The room goes dark.\n");
    } else 
    if (!old_light && new_light) {
	tell_from_inside(this_object(), "You can see again.\n");
    }
}

void set_light(int x) {
    int old = query_light();

    ::set_light(x);

    possible_light_change(old, query_light());
}

void adjust_light(int x) {
    int old = query_light();

    ::adjust_light(x);

    possible_light_change(old, query_light());
}

mixed direct_get_obj(object ob)
{
    if( this_object() == environment( this_body()))
	return "#A surreal idea.";
    return ::direct_get_obj(ob);
}

mapping lpscript_attributes() {
    return ([
      "area" : ({ LPSCRIPT_STRING, "setup", "set_area" }),
      "brief" : ({ LPSCRIPT_STRING, "setup", "set_brief" }),
      "default_exit" : ({ LPSCRIPT_STRING, "setup", "set_default_exit" }),
      "exits" : ({ LPSCRIPT_MAPPING, "setup", "set_exits" }),
      "state" : ({ LPSCRIPT_TWO, (: ({ "setup", "set_state_description(\"" + $1 + "\", \"" + $2[0] + "\")" }) :) }),
      "item" : ({ LPSCRIPT_SPECIAL, (: ({ "setup", "add_item(\"" + $1[0] + "\", \"" + implode($1[1..], " ") + "\")" }) :) })
    ]);
}

/* tweak the base long description to add the state stuff */
string get_base_long()
{
    string base = ::get_base_long();
    array fmt;
    int i;
    
    fmt = reg_assoc(base, ({ "\\$[A-Za-z_]*" }), ({ 1 }))[0];

    for (i = 1; i < sizeof(fmt); i++) {
	string tmp;
	
	if (tmp = query_state_desc(fmt[i][1..]))
	    fmt[i] = tmp;
    }

    base = implode(fmt, "");
    if (base[<1] != '\n') base += "\n";
    return base;
}


string long()
{
#ifdef OBVIOUS_EXITS_BOTTOM
    string objtally = show_objects();
    if( sizeof(objtally))
	objtally = "You also see:\n" + objtally;
    return sprintf("%sObvious Exits: %%^ROOM_EXIT%%^%s%%^RESET%%^\n%s",
      (dont_show_long() ? "" : simple_long()),
      show_exits(),
      objtally);
#else
    return sprintf("%s%s",
      (dont_show_long() ? "" : simple_long()),
      show_objects());
#endif
}

//:FUNCTION long_without_object
//This is used by things like furniture, so the furniture can use the
//same long as the room, but not see itself in the description.
string long_without_object(object o)
{
#ifdef OBVIOUS_EXITS_BOTTOM
    return sprintf("%sObvious Exits: %%^ROOM_EXIT%%^%s%%^RESET%%^\n%s",
      simple_long(),
      show_exits(),
      show_objects(o));
#else
    return sprintf("%s%s",
      simple_long(),
      show_objects(o));
#endif
}

void do_listen()
{
    write( "You hear nothing unusual.\n" );
}
