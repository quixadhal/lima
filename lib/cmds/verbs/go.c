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

mixed can_go_str(string str) {
    mixed value = environment(this_body())->can_go_somewhere(str);
    if (!stringp(value) && ( value == 1)) 
	return default_checks();
    if (!stringp(value) && (member_array(str, normal_directions) != -1))
        return "It doesn't appear you can go that way\n";
    return value;
}


void do_go_str(string str) {
    environment(this_body())->do_go_somewhere(str);
}

void create() {
    add_rules( ({ "STR" }), ({ "leave" }) );
    add_rules( ({ "WRD OBJ", "OBJ" }), ({ "climb", "enter" }) );
    add_rules( ({ "WRD OBJ" }), ({ "stand" }) );
//    add_rules( ({ "down OBJ", "up OBJ", "around OBJ:v", "to OBJ:v",
//                  "out of OBJ",
//                  "over OBJ", "on OBJ", "into OBJ", "in OBJ", "out OBJ", "OBJ" }),
//               ({ "climb", "enter" }) );
    clear_flag(NEED_TO_SEE);
    clear_flag(NEED_TO_BE_ALIVE);

}

    /*
    ** exit 1 -> go 1
    ** exit down OBJ -> go down OBJ
    ** exit up OBJ -> go up OBJ
    ** exit around OBJ -> go around OBJ
    ** exit to OBJ -> go to OBJ
    ** exit over OBJ -> go over OBJ
    ** exit on OBJ -> go on OBJ
    ** exit into OBJ -> go into OBJ
    ** exit in OBJ -> go in OBJ
    */
