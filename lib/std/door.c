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

#include <lpscript.h>

inherit COMPLEX_EXIT_OBJ;
inherit M_LOCKABLE;
inherit M_OPENABLE;
inherit M_KNOCKABLE;
inherit M_SIBLING;

/* Check function used by M_COMPLEX_EXIT */
mixed door_check(string dir, object who) {
  if (query_closed()) {
    this_body()->other_action( "$N $vtry to go $o, but the $o1 is closed.", 
			     dir, 
			     this_object());
    return sprintf("You try to go %s, but the %s is closed.\n",
		   query_obvious_description(),
		   the_short());
  }
  if(!load_object(query_method_destination("go")))
    return "Error, the destination does not exist, please mail the creator of this room";
  return 1;
}

//:FUNCTION update_state
// Internal use function.
// Update the state of both this door and its sibling.
void update_state(object ob) {
    m_openable::set_closed(ob->query_closed());
    ::set_locked(ob->query_locked(), ob->query_key_type());
}

//:FUNCTION update_sibling
// After a door is opened or closed, this is called, and in turn calls 
// do_on_open/close() in the door and its sibling. 
// See also m_sibling::update_sibling()
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

//:FUNCTION do_on_close
// Called when the door is closed.
void do_on_close() {
    if (!query_closed())
        object_event("The $o closes.\n");
#ifndef CLOSED_DOORS_ARE_OBVIOUS_EXITS
    set_hidden(1);
#endif    
}

//FUNCTION: do_on_open
// Called when the door is opened.
void do_on_open() {
    if (query_closed())
        object_event("The $o opens.\n");
#ifndef CLOSED_DOORS_ARE_OBVIOUS_EXITS
    set_hidden(0);
#endif    
}

varargs void on_clone(string dir, mixed rest...)
{
    object sib;
    if(!dir)
      dir=query_direction();
    if(!sizeof(rest))
      rest+=({query_method_destination("go")});
    complex_exit_obj::on_clone(dir,rest...);
    sib=get_sibling();
    if( sib ) update_state( sib );
}

//:FUNCTION set_door_direction
// Set the direction name the door uses.  This is also the name of the 
// resulting exit.
void set_door_direction(string direction)
{
  /* Start by removing the old stuff, if it exists */
  string olddir=query_obvious_description();

  if(olddir)
    {
      TBUG(olddir);
      remove_adj(olddir);
      remove_id(olddir);
    }
  set_obvious_description(direction);
  add_adj(direction);
  add_id_no_plural(direction);
}

//:FUNCTION set_door_destination
// Set the destination of the exit that the door covers. The destination can be
// anything M_COMPLEX_EXIT::set_method()'s destination argument will accept
void set_door_destination(mixed dest)
{
  /* This is to make the pathname relative to the room that the door is in 
   * rather than the door itself */
  if(stringp(dest)) {
    dest=(:absolute_path($(dest),environment()):);
  }
  set_method("go",dest,(:door_check:),
	     ({sprintf("$N $vleave through the $o.")}),
	     ({sprintf("$N $venter through the $o.")}));
    
  set_sibling_room(dest);
}

//:FUNCTION query_door_destination
// Return the destination of the door
mixed query_door_destination()
{
  return query_method_destination("go");
}

//:FUNCTION setup_exits
// Set the name of the direction and the location to which the door's exit 
// will lead.
// DEPRICATED! -- Use set_door_destination() and set_door_description() 
//                instead.
void setup_exits(string dir,string room) {
    add_method("go", room,(:door_check:));
    set_door_direction(dir);
}

//:FUNCTION setup_door
// Set up the key parts of the door.
// The first argument is the string to which the door responds and should 
// match its sibling, the second argument is the direction of the exit the 
// door covers, and the third argument is the object to which the exit leads.
// This should be called only once, from setup(). 
// DEPRECATED! -- Use set_door_destination, set_door_direction(), and 
//                set_sibling_ident() instead.

void setup_door(string ident, string dir, string room) {
    set_sibling_ident(ident);
    set_door_direction(dir);
    set_door_destination(room);
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


mapping lpscript_attributes()
{
  return ([ "ident" : ({ LPSCRIPT_STRING, "setup", "set_sibling_ident" }),
	    "direction" : ({ LPSCRIPT_STRING, "setup", "set_door_direction" }),
	    "destination" : ({ LPSCRIPT_STRING, "setup", "set_door_destination" }) ]);
}
