/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust wrote this, it's a lot like room.c, except it's
// for things like furniture and vehichles, which aren't rooms,
// but are locations.


#include <setbit.h>
#include <move.h>

inherit CONTAINER;
inherit M_ITEMS;
inherit "/std/room/exits";

int this_look_is_forced;

int stat_me() {
    printf("Location: %s [ %s ]\n\n",
      short(), implode(query_exit_directions(1), ", "));
    container::stat_me();
    return 1;
}


//:FUNCTION can_hold_water
//Return 1 if the object can hold water
/* by default, locations can hold water */
int can_hold_water()
{
    return 1;
}

void create()
{
    container::create();
    exits::create();
	
    set_light(DEFAULT_LIGHT_LEVEL);
    set_max_capacity(VERY_LARGE+LARGE);
    add_id_no_plural("here");
    set_preposition("on");
    set_default_exit((: "You're not going anywhere until you get out of the " 
		      + short() + ".\n" :));
}




string get_location_description()
{
  return (environment(this_object())->long_without_object(this_object()));
}

//:FUNCTION as_location_short
// Since locations double as objects and places to be, this is the short
// That goes at the top of a room description.  Usually it's the same as
// the room plus a modifier, like:  Grand Hall, in the camero

string get_brief()
{
  return sprintf("%s, %s the %s", environment(this_object())->short(),
		 query_prep(), short());
}

//:FUNCTION do_looking
//print out the description of the current room
void do_looking(int forced_look)
{
    // This probably shouldn't be done in a global variable,
    // instead it should be passed to long(), but long does
    // not take args anywhere else...
    this_look_is_forced = forced_look;

    if ( wizardp(this_body()) &&
      this_body()->query_shell_ob()->get_variable("show_loc") )
    {
	printf("[%s]\n", file_name(this_object()));
    }

    if ( environment(this_object())->query_light() < 1 )
    {
	write("Someplace dark\nIt is dark here.\n");
#ifdef ZORKMUD
	write("You might get eaten by a grue.\n");
#endif
    }
    else
    {
#ifdef OBVIOUS_EXITS
	printf("%s [exits: %s]\n", get_brief(), show_exits());
#else
	printf("%s\n", get_brief());
#endif

	write(wrap(get_location_description()));
    }
}



