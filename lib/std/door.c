/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * Design:
 *
 * BLOCKEXITS, OPENABLE, and LOCKABLE handle keeping our state for us
 * and keeping people from moving through the exit.  We do, however,
 * overload set_locked() and set_closed() so that we can inform our
 * other end.  We find our other end by finding the room we are in,
 * finding the destination in that direction, and searching that room
 * for a door that has the same 'identifier' as us.  We also cache
 * the object so we don't have to look that often under normal circumstances.
 * We also establish a move hook, so that when we are initially moved
 * to our room, we can check if our sibling already exists, and if so,
 * we copy it's state.  This way, as long as both objects are loaded,
 * they remain consistent, and if neither object is loaded, the first
 * one to load keeps it's default state, and the second one to load
 * copies the first.
 *
 * Note: in order for this to work, you have to call setup_door AFTER
 * set_locked and set_closed, otherwise it will happily update the sibling,
 * overriding the values we got from it.
 */

inherit OBJ;
inherit M_LOCKABLE;
inherit M_OPENABLE;
inherit M_BLOCKEXITS;
inherit M_KNOCKABLE;
inherit M_MESSAGES;

string our_ident;
string our_dir;
object cached_sibling;

private int block(string dir, object who) {
    if (query_closed()) {
	who->simple_action("$N $vtry to go $o, but the $o1 is closed.",
			   dir, this_object());
	return 0;
    }
    return 1;
}

void noisy(string arg) {
    object_event("The $o " + arg + ".\n");
}

int respond_to_door_ident(string id) {
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
	    if (ob2->respond_to_door_ident(our_ident)) {
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
	ob->update_state(this_object());
}

void update_state(object ob) {
    m_openable::set_closed(ob->query_closed());
    m_lockable::set_locked(ob->query_locked(), ob->query_key_type());
}

private void initial_move() {
    object ob;
    // when we initially get moved to a room, check if our sibling already
    // exits, and if so, get it's state.
    if (ob = get_sibling())
	update_state(ob);
}

void we_changed(string arg) {
    object ob;
    if (ob = get_sibling())
	ob->noisy(arg);
    update_sibling();
}

// should be called only once, from create.
void setup_door(string ident, string dir) {
    object ob;

    our_ident = ident;
    our_dir = dir;

    add_adj(dir);
    set_unique(1); // doors should have a unique enough description that
	           // we can refer to them with 'the'

    add_block(dir);
    set_block_action( (: block :) );
    add_hook("open", (: we_changed, "opens" :));
    add_hook("close", (: we_changed, "closes" :));
    add_hook("move", (: initial_move :));
}

varargs void set_locked(string x, string y) {
    m_lockable::set_locked(x, y);
    update_sibling();
}

void set_closed(int x) {
    m_openable::set_closed(x);
    update_sibling();
}

void knock_knock(string s)
{
  object sibling;

  if(is_open())
    {
      write("There is no need, the door is already open.\n");
      return;
    }
  sibling = get_sibling();
  if(sibling)
    {
      tell_room(environment(sibling), "There is a knock at the door.\n");
    }
  ::knock_knock(s);
}


mixed direct_get_obj( object obj )
{
    return "#Opening it would be easier.\n";
}
