/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** simple_ob.c -- an object that interacts with verbs very simply
**
** This object is used for more complex interactions (compared to the
** items that are managed by a FAKE_ITEM_MGR).  This object can respond
** with different messages when the user attempts to interact with it
** with verbs.  A message can be returned for each different verb.
**
** Adjectives are specified for the object using the "pseudo" verb "adjs".
** The long description is set from the response given for the "look" verb
** (if a response is provided).  Otherwise, the standard default is used
** (which is something like, "You see nothing special about the ...")
**
** Note that this object is attached to the parent, so it won't actually
** show up in the parent's inventory; the parent should direct the user
** to this object through some aspect of its description.
**
** This object is typically never used directly, but instead via the
** M_ITEMS module (normally inherited by locations the user might visit
** such as a ROOM or a NON_ROOM).
**
** 970114, Deathblade: trimmed and commented
** ???, Rust: created
*/

inherit BASE_OBJ;

private nosave mapping msgs = ([]);

varargs void create(mapping long, string array ids, object dest)
{
    if (!clonep()) return;
    ::create();

    set_attached(1);
    msgs = long;

    if(msgs["look"])
	set_long(msgs["look"][<1] == '\n' ? msgs["look"] : 
		 msgs["look"]+"\n");
    if(msgs["adjs"])
    {
	if ( arrayp(msgs["adjs"]) )
	    add_adj(msgs["adjs"]...);
	else
	    add_adj(msgs["adjs"]);
	map_delete(msgs, "adjs");
    }

    set_id(ids...);
      
    parse_refresh();

    move_object(previous_object());
}

// respond to all interaction with self as a direct object of the verb
mixed direct_verb_rule(string verb, string rule, mixed args)
{
    string s = msgs[verb];
    if(s)
	return s[<1] == '\n' ? s : s+"\n";

    /* can't use that verb on us... */
    return 0;
}
