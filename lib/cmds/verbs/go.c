/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** go.c
** 072197 Completely rewritten by Rassilon in the great exit rewrite.
*/

#include <verbs.h>

inherit NVERB_OB;

string array normal_directions = ({ "north", "south", "west", "east",
                                    "northwest", "northeast", 
                                    "southwest", "southeast", 
                                    "up", "down" });

// All of these rules direct all movement descisions through
// environment(this_body()).


mixed can_go_wrd(string prep) {
    if (member_array(prep, master()->parse_command_prepos_list()) != -1)
    {
        if (is_normal_direction(prep)) return 0;
        return 1;
    }
   else
      return 0;
}

void do_go_word_obj(string prep, object ob) {
    environment(this_body())->do_go_obj(ob, prep);
}

void do_go_wrd_obj(string prep, object ob) {
    // The driver calls this for WRD OBJ rules instead of do_go_word_o()
    do_go_word_obj(prep, ob);
}

void do_go_obj(object ob) {
    environment(this_body())->do_go_obj(ob, 0);
}

void do_go() {
    environment(this_body())->do_go_obj(environment(this_body()), 0);
}

void do_go_wrd(string prep) {
    do_go_word_obj(prep, environment(this_body()));
}

mixed can_go_str(string str) {
    mixed value = environment(this_body())->can_go_somewhere(str);
    if (!stringp(value) && ( value == 1)) 
	return default_checks();
    if (!stringp(value) && (member_array(str, normal_directions) != -1))
        return "It doesn't appear you can go that way\n";
    return value;
}


mixed can_go_obj_str(object ob, string str) {
    return can_go_str(str);
}

void do_go_str(string str) {
    environment(this_body())->do_go_somewhere(str);
}

void create()
{
   add_rules( ({ "STR" }), ({ "leave" }) );
   add_rules( ({ "WRD OBJ" }) );
   clear_flag(NEED_TO_SEE);
   clear_flag(NEED_TO_BE_ALIVE);
}
