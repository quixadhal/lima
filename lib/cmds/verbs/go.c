/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** go.c
** 072197 Completely rewritten by Rassilon in the great exit rewrite.
** 020999 Revised by Tigran in his exit modifications
*/

#include <verbs.h>

inherit NVERB_OB;

string array normal_directions = ({ "north", "south", "west", "east",
                                    "northwest", "northeast", 
                                    "southwest", "southeast", 
                                    "up", "down" });


void do_go_obj(object ob) 
{
  ob->do_verb_rule("go", "OBJ", ob);
}

void do_go_word_obj(string prep, object ob)
{
  ob->do_verb_rule("go", "WRD OBJ", prep, ob);
}

void do_go_str(string str) 
{
  environment(this_body())->do_go_str(str);
}

mixed can_go_str(string str) {
  mixed value = environment(this_body())->can_go_str(str);
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
