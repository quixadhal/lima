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
    mixed dest;

    /*
    ** if there is no outside object (or the env isn't an inside), then
    ** driving is illegal :-)  Also, rule out non-vehicles.
    */
    if ( !ob || !ob->is_vehicle() )
	return 0;

    dest = environment(ob)->get_exits()[str];
    if ( undefinedp(dest) )
    {
	int is_normal = (member_array(str, normal_dirs) != -1);
	
	if ( environment(ob)->is_default_exit(str, is_normal) )
	    return 1;
	if ( is_normal )
	    return "It doesn't seem possible to go that direction.\n";
	return 0;
    }

    /* let's just assume they can drive anywhere */
    return 1;
}

void do_drive_str(string str)
{
    environment(this_body())->get_outside()->go_somewhere(str);
}

mixed *query_verb_info()
{
    return ({ ({ "STR" }) });
}
