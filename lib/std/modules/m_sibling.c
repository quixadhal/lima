/* Do not remove the headers from this file! see /USAGE for more info. */
// Folded out from DOOR.   --OH.


string our_ident;
string our_dir;
object cached_sibling;


int respond_to_sibling_ident(string id) {
    return our_ident == id;
}

object get_sibling() {
    if(!environment()) { ZABUG("ENV 0"); return 0; }
    if (!cached_sibling) {
	string tmp;
	object ob;
    
	tmp = environment()->query_exit_value(our_dir);
        if(!tmp) { ZABUG("NO EXIT VAL"); return 0; }
	ob = load_object(tmp);
        if(!ob) { ZABUG("NO LOAD"); return 0; }

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


