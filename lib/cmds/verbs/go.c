/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** go.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

//###should be shared somehow with drive.c
string *normal_dirs = ({ "north", "south", "east", "west", "northwest", "northeast", "southwest", "southeast" });

mixed can_go_wrd_obj(string prep, object ob) {
    return 1;
}

void do_go_wrd_obj(string prep, object ob) {
    ob->go(prep);
}

mixed can_go_str(string str) {
    int is_normal = (member_array(str, normal_dirs) != -1);
    mixed value;
// Be careful what errors you return here since "go " + str is tried for
// all input
    
    value = environment(this_body())->query_exit_value(str, is_normal);
    if (stringp(value) && value[0] == '#')
	return value[1..];
    if (value)
	return 1;

    if (is_normal) {
	mixed ret = call_other(environment(this_body()), "can_go_" + str);
	if (!ret) ret = "It doesn't seem possible to go that direction.\n";
	return ret;
    }

    return 0;
}

void do_go_str(string str) {
    this_body()->go_somewhere(str);
}

mixed * query_verb_info()
{
    return ({ ({ 1, "down OBJ", "up OBJ", "around OBJ:v", "to OBJ:v",
    "over OBJ", "on OBJ", "into OBJ", "in OBJ" }) });

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
}
