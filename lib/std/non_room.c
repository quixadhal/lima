/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust wrote this, it's a lot like room.c, except it's
// for things like furniture and vehichles, which aren't rooms,
// but are locations.

/* Modified by Tigran in his exit rewrite - 2/14/99*/

#include <setbit.h>
#include <move.h>

inherit CONTAINER;
inherit M_COMPLEX_EXIT;
inherit M_ITEMS;
inherit M_EXIT;

int is_player_inside();

string stat_me()
{
    return sprintf("Location: %s [ %s ]\n\n",
      short(), implode(query_exit_directions(1), ", ")) +
    container::stat_me() + m_complex_exit::stat_me();
}

//:FUNCTION can_hold_water
//Return 1 if the object can hold water
/* by default, locations can hold water */
int can_hold_water()
{
    return 1;
}

/* The purpose of this function is to be called/set as a check to NON_ROOM 
 * objects which move the PC into the NON_ROOM.  Don't want to do that if they
 * are already inside. */
mixed enter_check()
{
    if(environment(this_body())!=environment(this_object()))
	return "You are already there.";
    return 1;
}

//:FUNCTION parent_environment_accessible
//Return 1 if the outside should be included in the parse
//Non-rooms default to returning 1
int parent_environment_accessible() {
    return 1;
}

string get_location_description()
{
    return environment(this_object())->long_without_object(this_object());
}

//:FUNCTION as_location_short
// Since locations double as objects and places to be, this is the short
// That goes at the top of a room description.  Usually it's the same as
// the room plus a modifier, like:  Grand Hall, in the camero

/* This needs some serious work. -- Tigran */
string get_brief()
{
    return sprintf("%s, %s the %s", environment(this_object())->short(),
      query_default_relation(), short());
}

//:FUNCTION do_looking
//print out the description of the current room
void do_looking(int force_long_desc)
{
    //### how to use force_long_desc ??

    if ( wizardp(this_user()) && get_user_variable("show_loc") )
    {
	printf("[%s]\n", file_name(this_object()));
    }

    if ( environment(this_object())->query_light() < 1 )
    {
	write("Someplace dark\nIt is dark here.\n");
    }
    else
    {
#ifdef OBVIOUS_EXITS
	printf("%%^ROOM_SHORT%%^%s%%^RESET%%^" +
	  " [exits: %%^ROOM_EXIT%%^%s%%^RESET%%^]\n",
	  get_brief(), show_exits());
#else
	printf("%%^ROOM_SHORT%%^%s%%^RESET%%^\n",
	  get_brief());
#endif

	write(get_location_description());
    }
}

int is_player_inside() {
    return (environment(this_body()) == this_object());
}

string show_exits()
{
    object env = environment();
    return (env ? env->show_exits() : "none");
}

