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
string *normal_dirs = ({ "north", "south", "east", "west", "northwest", "northeast", "southwest", "southeast" });

mixed can_drive_str(string str)
{
    object ob = environment(this_body())->get_outside();
    mixed value;
    int is_normal;

    /*
    ** if there is no outside object (or the env isn't an inside), then
    ** driving is illegal :-)  Also, rule out non-vehicles.
    */
    if ( !ob || !ob->is_vehicle() )
	return 0;

    is_normal = (member_array(str, normal_dirs) != -1);
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

void do_drive_str(string str)
{
    environment(this_body())->get_outside()->go_somewhere(str);
}

mixed *query_verb_info()
{
    return ({ ({ "STR" }) });
}
