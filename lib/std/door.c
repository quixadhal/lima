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

inherit COMPLEX_EXIT_OBJ;
inherit M_LOCKABLE;
inherit M_OPENABLE;
inherit M_KNOCKABLE;
inherit M_SIBLING;

mixed door_check(string dir, object who) {
  if (query_closed()) {
    this_body()->other_action( "$N $vtry to go $o, but the $o1 is closed.", 
			     this_body(),
			     dir, 
			     this_object());
    return sprintf("You try to go %s, but the %s is closed",
		   query_obvious_description(),
		   dir);
  }
  if(!load_object(query_method_destination("go")))
    return "Error, the destination does not exist, please mail the creator of this room";
  return 1;
}

void update_state(object ob) {
    m_openable::set_closed(ob->query_closed());
    ::set_locked(ob->query_locked(), ob->query_key_type());
}

void update_sibling()
{
    object sibling = get_sibling();
    // Needed for initial load ...\
    if( !sibling )
        return;

    // Magic ...
    if( is_open())
        sibling->do_on_open();
    else sibling->do_on_close();
    ::update_sibling();
}

void do_on_close() {
    if (!query_closed())
        object_event("The $o closes.\n");
}

void do_on_open() {
    if (query_closed())
        object_event("The $o opens.\n");
}

varargs void on_clone(string dir, mixed rest...)
{
    object sib;
    complex_exit_obj::on_clone(dir,rest...);
    m_sibling::on_clone(dir,rest...);
    sib=get_sibling();
    if( sib ) update_state( sib );
}


// Break this out so other kinds of doors can overload this
void setup_exits(string dir,string room) {
    add_method("go", room,(:door_check:));
    set_obvious_description(dir);
}

// should be called only once, from create.
void setup_door(string ident, string dir, string room) {
    add_adj(dir);
    /* It is necessary to add the direction as an id too because people 
     * will still 'go west' for the west door */
    add_id_no_plural(dir);
    set_unique(1); // doors should have a unique enough description that
    // we can refer to them with 'the'
    respond_to_sibling_ident(ident);
    setup_exits(dir,room);
}


varargs void set_locked(string x, string y) {
    m_lockable::set_locked(x, y);
    update_sibling();
}

void set_closed(int x) {
    ::set_closed(x);
    update_sibling();
}

void do_knock()
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
	tell_environment( sibling, "There is a knock at the door.\n" );
    }
    ::do_knock();
}


mixed direct_get_obj( object obj )
{
    return "#Opening it would be easier.\n";
}



