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
// Sept. 5 - Yaynu added weather settings
// Satan@ArcadiaF 10-29-95 Added the OBVIOUS_EXITS_BOTTOM, casue I don't like
//      the way the exits look on the top, and Beek didn't want to fix it.
// Satan@ArcadiaF 10-30-95 Added set_hidden_exits(). This makes a list that
//      of exits that are hidden from all in the room.
//      Wizards can see DARK_EXITS as they are denoted with a *name format.
// 951113, Deathblade: removed some obsolete vars; made them all private

#include <mudlib.h>
#include <playerflags.h>
#include <setbit.h>
#include <move.h>
#include <hooks.h>

inherit CONTAINER;
inherit M_ITEMS;
inherit __DIR__ "room/weather";


// global vars

private static string base;
private static string remote_desc;
private static mixed def_exit;
private static mapping exits = ([]);
private static string array hidden_exits = ({});
private static mapping exit_msg = ([]);
private static mapping enter_msg = ([ ]);
private static int total_light;
private static string map_type;

//:FUNCTION query_lit
//Return the amount of light visible in this room
int query_lit() { return total_light + query_light(); }

/* Never, EVER, under pain of death, call this function.  For
   internal mudlib use ONLY! */
void adjust_light(int x) 
{
    total_light += x;
}

int stat_me() {
    printf("Room: %s [ %s ]\n\n",
      short(), implode(keys(exits),", "));
    container::stat_me();
    return 1;
}

//:FUNCTION set_brief
//Set the name of the room seen at the top of the description and in brief mode
void set_brief(string str) {
    set_proper_name(str);
}

//:FUNCTION can_hold_water
//Return 1 if the object can hold water
/* by default, rooms can hold water */
int can_hold_water()
{
    return 1;
}

void create(){
    container::create();

    base = file_name(this_object());
    base = base[0..strsrch(base, '/', -1)];
    set_max_capacity( 1000000 );
    add_id_no_plural("ground", "here");
    add_id("environment", "room");
}

//:FUNCTION get_exits
//Return the exits mapping
mapping get_exits()
{
    return exits;
}

//:FUNCTION get_default_exit
//Return the room's default exit (the message given when someone goes a
//direction with no exit)
mixed get_default_exit() {
    return def_exit;
}

//:FUNCTION set_default_exit
//Set the default exit message (the message given when someone goes a direction
//with no exit).  This should be a string or a function ptr returning a string.
void set_default_exit(mixed value) {
    def_exit = value;
}

//:FUNCTION is_default_exit
//Return whether the given direction can reasonably have an implicit
//destination.  Only "normal" directions have implicit destinations,
//and the room must have a default exit.
int is_default_exit(string str, int is_normal_direction) {
    return is_normal_direction && def_exit;
}

//:FUNCTION get_exit
//Get the destination of a specified exit
string get_exit(string dir) {
    mixed tmp;
    int i;

    tmp = exits[dir];
    if (!tmp) return 0;
    if (stringp(tmp)) return tmp;
    for (i=0; i<sizeof(tmp); i++) {
	if (stringp(tmp[i])) return tmp[i];
    }
    return 0;
}

//:FUNCTION show_exits
//Return a string giving the names of exits for the obvious exits line
string show_exits()
{
    string exit_str;
    string* exit_names;

    exit_names = keys(exits) - hidden_exits;
    exit_str = ((sizeof(exit_names)) ? implode(exit_names,", ") : "none");

#ifdef WIZARDS_SEE_HIDDEN_EXITS
    if ( wizardp(this_user()) && sizeof(hidden_exits) )
    {
       exit_str += ", *" + implode(hidden_exits, ", *");
    }
#endif

    return exit_str;
}

//:FUNCTION do_looking
//print out the description of the current room
void do_looking()
{
    if ( wizardp(this_body()) &&
      this_body()->query_shell_ob()->get_variable("show_loc") )
    {
	printf("[%s]\n", file_name(this_object()));
    }

    if ( query_lit() < 1 )
    {
	write("Someplace dark\nIt is dark here.\nYou might get eaten by a grue.\n");
    }
    else
    {
#ifdef OBVIOUS_EXITS
	printf("%s [exits: %s]\n", short(), show_exits());
#else
	printf("%s\n", short());
#endif

	if (!this_body()->test_flag(F_BRIEF)) 
	    write(wrap(long() + "\n"));

	write("\n");
    }
}

//:FUNCTION show_objects
//Return a string describing the objects in the room
string show_objects()
{
    object *obs;
    string user_show;
    string obj_show;
    string str;
    int n;

    obs = filter(all_inventory() - ({ this_body() }), 
		 (: $1->is_visible() :));
    n = sizeof(obs);
    user_show = "";
    obj_show = "";

    while (n--) {
	if (obs[n]->is_living()) {
	    str = obs[n]->in_room_desc();
	    if(strlen(str)) 
	      {
		obj_show += str + "\n";
	      }
	} else {
	    if (!duplicatep(obs[n])) {
		if ((str = obs[n]->show_in_room()) && strlen(str)) {
		    obj_show += str + "\n";
		}
		if (obs[n]->inventory_visible())
		    obj_show += obs[n]->inventory_recurse();
	    }
	}
    }

    if (user_show != "") obj_show += "\n"+user_show;
    return wrap(obj_show);
}

/*
** map_type is used to indicate the map that a room is on (used on muds
** with map systems)
*/
void set_map(string type)
{
    map_type = type;
}
string print_map()
{
    return map_type;
}

//### maybe have a better interface than global vars
string query_enter_msg(string arg)
{
    return enter_msg[arg]; 
}

string query_exit_msg(string arg)
{
    return exit_msg[arg]; 
}

void set_enter_msg(string dir, string msg)
{
  enter_msg[dir] = msg;
}

void set_exit_msg(string dir, string msg)
{
  exit_msg[dir] = msg;
}

nomask string long()
{
#ifdef OBVIOUS_EXITS_BOTTOM
    return sprintf("%s%s\nObvious Exits: %s\n%s",
                   container::simple_long(),
                   weather(),
                   show_exits(),
                   show_objects());
#else
    return sprintf("%s%s%s",
		   container::simple_long(),
		   weather(),
		   show_objects());
#endif
}

//:FUNCTION add_exit
//Adds a single exit to the room.  The direction should be an exit name,
//and the value should be a filename or a more complex structure as
//described in the exits doc.
void add_exit(mixed direction, mixed value)
{
    exits[direction] = value;
}

//:FUNCTION delete_exit
//Remove a single exit from the room.  The direction should be an exit
//name.
void delete_exit(mixed direction)
{
    map_delete(exits, direction);
}

//:FUNCTION set_exits
//Sets the exit mapping of a room.  The keys should be exit names, the values
//should be either filenames or more complex structures described in the
//exits doc
void set_exits( mapping new_exits )
{
    if ( mapp(new_exits) )
	exits = new_exits;
}

//:FUNCTION set_hidden_exits
//This is the list of exits to NOT be shown to the mortals in the room.
void set_hidden_exits( string array exits_list ... )
{
  hidden_exits = exits_list;
}

string query_name()
{
    return "the ground";
}

//### I think this should be torched :-)
// I don't.
// This should be overloaded if you want to be able to give different
//descs from different rooms
void remote_look(object o)
{
    if ( remote_desc )
    {
	printf("%s\n", remote_desc);
    }
    else
    {
	printf("You can't seem to make out anything.\n");
    }
}

string query_base()
{
    return base;
}

void set_remote_desc(string s)
{
    remote_desc = s;
}







