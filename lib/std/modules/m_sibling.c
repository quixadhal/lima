/* Do not remove the headers from this file! see /USAGE for more info. */
// Folded out from DOOR.   --OH.


private string our_ident;
private function sibling_room;
private object cached_sibling;

// DEPRECATED
void setup_sibling(string ident, mixed room) {
    if(ident)
	our_ident = ident;
    if (stringp(room))
      sibling_room = (:absolute_path($(room), environment()):);
    else
      sibling_room = room;
    
    cached_sibling = 0;
}

void set_sibling_ident(string ident) {
    if(ident)
	our_ident = ident;

    cached_sibling = 0;
}

void set_sibling_room(mixed room) {
    if (stringp(room))
      sibling_room = (:absolute_path($(room), environment()):);
    else
      sibling_room = room;
    
    cached_sibling = 0;
}

int respond_to_sibling_ident(string id) {
    return our_ident == id;
}

object get_sibling() {
    if(!environment()) { ZABUG("ENV 0"); return 0; }
    if (!cached_sibling) {
	object ob;
	if(!sibling_room || !evaluate(sibling_room))
	    return 0;
	ob = load_object(evaluate(sibling_room));
	if(!ob) { TBUG("NO LOAD"); return 0; }

	foreach (object ob2 in all_inventory(ob)) {
	    if (ob2->respond_to_sibling_ident(our_ident)) {
		cached_sibling = ob2;
		break;
	    }
	}
    }
    return cached_sibling;
}

void update_sibling() {
    object ob;
    if (ob = get_sibling())
	ob->update_state( this_object() );
}

void update_state(object ob) {
    // Insert stuff to sync
}

private void initial_move() {
    object ob;
    // when we initially get moved to a room, check if our sibling already
    // exits, and if so, get it's state.
    if (ob = get_sibling())
	update_state(ob);
}

/* 
 * This is here to properly setup the sibling room.  If it's done in setup()
 * or mudlib_create() etc, it is called before the door is moved into its
 * environment which makes things work 'not quite right'
 */

/*** Deprecated by change to funptr for sibling_room -- Marroc
varargs void on_clone(string direction,string room, mixed rest...)
{
    setup_sibling(0,room);
}
***/
