/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** fake_item_mgr -- manage "fake" items
**
** This object represents a bunch of items in a room that can only be
** looked at.  They don't appear in the room inventory (because of the
** call to set_attached()), so it is expected that the user discovers
** these through the main room description or other hints in a room.
**
** These objects only interact with the "look" verb.  For more interaction,
** use SIMPLE_OB or a full object implementation.
**
** This object is typically never used directly, but instead via the
** M_ITEMS module (normally inherited by locations the user might visit
** such as a ROOM or a NON_ROOM).
**
** 970114, Deathblade: cleaned up a bit and documented.
** ????, Rust: created.
*/

inherit BASE_OBJ;

private static mapping descs = ([]);
private static string my_name;


varargs void create(string long, string array ids, object dest)
{
    if (!clonep()) return;
    ::create();

    set_attached(1);

    set_long(long);

    set_id(ids...);
    foreach(string id in ids)
	descs[id] = long;
      
    parse_refresh();

    move_object(previous_object());
}


//:FUNCTION get_item_desc
//Return the appropriate description for the given id.
string get_item_desc(string id)
{
    return descs[id];
}

//:FUNCTION add_simple_fake_item
//Add an item into the manager.  It should have a long description that will
//be used to respond to the specified list of ids.
void add_simple_fake_item(string long, string array ids)
{
    set_id(ids...);
    foreach(string id in ids)
	descs[id] = long;
}

// respond to all verbs/rules when used as a direct object
int direct_verb_rule(string verb, string rule, mixed args...)
{
    /*
    ** The args contain: ob1..obN, verb, name1..nameN
    **
    ** Being a direct_xxx rule, we are ob1.  Therefore, we should grab
    ** name1 and pretend that is our name.
    */
    my_name = args[sizeof(args)/2 + 1];

    /* whatever is being done... can't be done */
    return 0;
}

// in case somebody asks what we are... use the last interaction
//### this is HOKEY... should be fixed.
string the_short()
{
    return "the " + my_name;
}
