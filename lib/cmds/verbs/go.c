/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** go.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

int need_to_be_alive() {
    return 0;
}

int need_to_see() {
    return 0;
}

//###should be shared somehow with drive.c
string array normal_dirs = ({  "up", "down", "north", "south", "east", "west", "northwest", "northeast", "southwest", "southeast" });

void do_go_wrd_obj(string prep, object ob) {
    ob->go(prep);
}

// This is one of the most heinous functions in this lib. Sorry.
mixed can_go_str(string str) {
    int is_normal = (member_array(str, normal_dirs) != -1);
    mixed value;
    object env = environment(this_body());

// Be careful what errors you return here since "go " + str is tried for
// all input
    
    value = env->query_exit_value(str, is_normal);
    if (stringp(value) && value[0] == '#')
      {
	string other_value;
        if(environment(env) && env->can_travel() && (other_value = 
	   env->query_final_exit_value(str,is_normal)))
	  {
	    if(stringp(other_value) && other_value[0] != '#')
	      {
		return default_checks();
	      }
	    else
	      {
		if(stringp(other_value))
		  return other_value[1..];
		else
		  return default_checks();
	      }
	  }
	return value[1..];
      }
    if (value)
	return default_checks();

    if (is_normal) {
	mixed ret = call_other(environment(this_body()), "can_go_" + str);
	if (!ret) ret = "It doesn't seem possible to go that direction.\n";
	return ret;
    }
}

void do_go_str(string str) {

    this_body()->go_somewhere(str);
}

array query_verb_info()
{
    return ({ ({ "STR" }), ({ "leave" }), ({ "down OBJ", "up OBJ", "around OBJ:v", "to OBJ:v", "over OBJ", "on OBJ", "into OBJ", "in OBJ", "STR" }) });

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
