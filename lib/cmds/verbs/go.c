/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** go.c
** 072197 Completely rewritten by Rassilon in the great exit rewrite.
** 020999 Revised by Tigran in his exit modifications
*/

#include <verbs.h>

inherit VERB_OB;

string array normal_directions = ({ "north", "south", "west", "east",
  "northwest", "northeast", 
  "southwest", "southeast", 
  "up", "down" });


void do_go_obj(object ob) 
{
    ob->do_verb_rule("go", "OBJ", ob);
}

void do_go_wrd_obj(string prep, object ob)
{
    ob->do_verb_rule("go", "WRD OBJ", prep, ob);
}

void do_go_str(string str) 
{
    object env = environment(this_body());

    if (this_body()->query_driving_vehicle())
	env = environment(env);

    env->do_go_str(str);
}

mixed can_go_wrd_obj(string prep, object ob)
{
    return "You can't do that!\n";
}

mixed can_go_str(string str) {
    object env = environment(this_body());
    mixed value;

    if (this_body()->query_driving_vehicle())
	env = environment(env);

    value = env->can_go_str(str);
    if (!stringp(value) && ( value == 1))
	return default_checks();
    if (!stringp(value) && (member_array(str, normal_directions) != -1))
	return "It doesn't appear you can go that way\n";
    return value;
}

void create()
{
    add_rules( ({ "STR" }), ({ "leave" }) );
    add_rules( ({ "WRD OBJ" }) );
    add_rules( ({ "OBJ" }) );
    clear_flag(NEED_TO_SEE);
    clear_flag(NEED_TO_BE_ALIVE);
}
