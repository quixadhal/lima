/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** drive.c
**
** Used to drive vehicles
**
** 951123, Deathblade: created
*/

inherit VERB_OB;

//###should be shared somehow with go.c
string array normal_dirs = ({ "north", "south", "east", "west", "northwest", "northeast", "southwest", "southeast" });


private mixed can_go_that_way(object ob, string str)
{
    mixed value;
    int is_normal;

    is_normal = (member_array(str, normal_dirs) != -1);
    if(!environment(ob))
      return "You need to be behind the wheel of something in order "
	"to drive.\n";
    value = environment(ob)->query_exit_value(str, is_normal);
    if (stringp(value) && value[0] == '#')
	return value[1..];
    
    /* let's just assume they can drive anywhere */
    if (value)
	return 1;

    if (is_normal)
	return "It doesn't seem possible to go that direction.\n";

    return 0;
}


mixed can_drive_str(string str)
{
  return can_go_that_way(environment(this_body()), str);
}

void do_drive_str(string str)
{
    environment(this_body())->go_somewhere(str);
}

mixed can_drive_obj()
{
  return "You need to specify a direction.\n";
}
 
int can_drive_obj_str(object ob, string str)
{
  if(!ob)
    return 0;
  return can_go_that_way(ob, str);
}

void do_drive_obj_str(object o, string str)
{
  o->go_somewhere(str);
}


array query_verb_info()
{
    return ({ ({ "STR", "OBJ", "OBJ STR" }) });
}
