/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust.  This one is for simple items there mainly for ornamentation.

inherit BASE_OBJ;
inherit __DIR__ "object/vsupport";

private static mapping descs = ([]);
private static string my_name;


varargs void create(string long, string array ids, object dest)
{
    if (!clonep()) return;
    ::create();

    set_attached(1);
    if(strlen(long))
	set_long(long);
    if(arrayp(ids))
	set_id(ids...);
    if(stringp(long))
	foreach(string id in ids)
	    descs[id] = long;
      
    parse_refresh();
    if(!objectp(dest))
	move_object(previous_object());
    else
	move_object(dest);
}



string get_item_desc(string id)
{
    return descs[id];
}

void add_simple_fake_item(string long, string array ids)
{
    set_id(ids...);
    foreach(string id in ids)
	descs[id] = long;
}

int direct_verb_rule(string verb, string rule, mixed args)
{
    for(int i = 0; i < sizeof(args)/2; i++)
    {
	if (args[i] == this_object())
	{
	    my_name = args[sizeof(args)/2 + i];
	}
	break;
    }
}

string the_short()
{
    return "the " + my_name;
}
