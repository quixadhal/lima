/* Do not remove the headers from this file! see /USAGE for more info. */
// container.c
// possibly the grossest piece of code in the mudlib.

#include <move.h>
#include <setbit.h>
#include <driver/origin.h>
#include <hooks.h>

inherit OBJ;

private int max_capacity = 10;
private static int capacity;
private mapping objects;
private string main_prep = "in";

private int contained_light;
private int contained_light_added;


//:FUNCTION container
//Returns 1 if an object is a container
int container()  { return 1; }

//:FUNCTION ob_state
//Determine whether an object should be grouped with other objects of the
//same kind as it.  -1 is unique, otherwise if objects will be grouped
//according to the return value of the function.
int ob_state() {
/* if we have an inventory we should be unique */
    if (first_inventory(this_object())) return -1;
    return 0;
}

//:FUNCTION query_capacity
//Returns the amount of mass currently in a container
int query_capacity()
{
  return capacity;
}

#ifdef USE_MASS
int query_mass()
{
  return capacity + ::query_mass();
}
#endif

#ifdef USE_SIZE
int query_aggregate_size()
{
  return capacity + :: get_size();
}
#endif

//:FUNCTION set_capacity
//Sets the amount of mass inside a container; this function should probably
//never be called as the value is kept internally.
static void
set_capacity( int c )
{
//### what calls this?  Is it needed?
  capacity = c;
}

//:FUNCTION query_max_capacity
//Find out the maximum capacity of an object
int query_max_capacity()
{
    return max_capacity;
}

//:FUNCTION receive_object
//Determine whether we will accept having an object moved into us;
//returns a value from <move.h> if there is an error
mixed receive_object( object target, string relation )
{
    int x,m;
    object ob;

    if( target == this_object() )
	return "You can't move an object inside itself.\n";

    if( origin() != ORIGIN_LOCAL )
    {
        /* allow a matching relation or newly cloned objects */
        if ( relation && relation != main_prep && relation != "#CLONE#" )
            return "You can't put things " + relation + " that.\n";
    }
#ifdef USE_SIZE
    x = target->get_size();
#else
    x=target->query_mass();
#endif
//    printf("cap=%O  max=%O  x=%O\n",query_capacity(),max_capacity,x);
    if ( (m=(query_capacity())+x) > max_capacity )
    {
	return MOVE_NO_ROOM;
    }

    set_capacity( m );

    return 1;
}

//:FUNCTION release_object
//Prepare for an object to be moved out of us; the object isn't allowed
//to leave if we return zero or a string (error message)
varargs mixed release_object( object target, int force )
{
#ifdef USE_SIZE
    set_capacity( query_capacity() - (target->get_size()) );
#else
#ifdef USE_MASS
    set_capacity( query_capacity() - (target->query_mass()) );
#endif
#endif //USE_SIZE
    return 1;
}

//:FUNCTION set_max_capacity
//Change the maximum capacity of an object.
void
set_max_capacity(int x) {

/* You can mask this if you want it to be changed externally in
   your room, but this is to prevent pranks
 */
//### why not just static it?
    if( origin() != ORIGIN_LOCAL)
	return 0;
    max_capacity = x;
}

//:FUNCTION look_in
//returns a string containing the result of looking inside (or optionally
//a different relation) of the object
string look_in( string relation )
{
    object* obs;
    string inv;
    mixed ex;
    
//:HOOK prevent_look
//A set of yes/no/error hooks which can prevent looking <relation> OBJ
//The actual hook is prevent_look_<relation>, so to prevent looking
//in something use prevent_look_in.
    ex = call_hooks("prevent_look_" + relation, HOOK_YES_NO_ERROR);
    if (!ex) ex = "That doesn't seem possible.";
    if (stringp(ex))
	return ex;
    
    if (relation && relation != main_prep)
        return "There is nothing there.\n";

    inv = inv_list(this_object(), 0, 1);
    if ( !inv )
	inv = "  nothing";
    
    return (sprintf( "%s %s you see: \n%s\n",
	capitalize(main_prep), short(),
	inv ));
}

void create() {
    objects = ([]);
    set_capacity( 0 );
    ::create();
}

//:FUNCTION inventory_visible
//Return 1 if the contents of this object can be seen, zero otherwise
int inventory_visible()
{
    if ( !is_visible() )
	return 0;

//### this should go!! short() should never return 0
    if (!short()) return 0;

    if ( test_flag(TRANSPARENT) )
	return 1;

    return !this_object()->query_closed();
}

//:FUNCTION inventory_accessible
//Return 1 if the contents of this object can be touched, manipulated, etc
int inventory_accessible() {
  if (!is_visible()) return 0;
  if (!short()) return 0;
  return !this_object()->query_closed();
}

//:FUNCTION inventory_header
//Returns a string header to put before inventory lists
string inventory_header() {
    return "It contains:";
}

string long()
{
    string res;
    string contents;

    res = ::long();
    contents = inv_list(this_object(), 1, 1);
    if (inventory_visible() && contents)
	res += inventory_header() + "\n" + contents;
    return res;
}

//:FUNCTION simple_long
//Return the long description without the inventory list.
string simple_long() {
    return ::long();
}

void reset(){
    int tally, num;
    mixed *rest;
    mixed value;
    string file, name;
    mixed oids;
    object *inv;
    int ret;
    
    if(!mapp(objects))
	return;

    inv = all_inventory();
    foreach (file, value in objects) {
	if (intp(value)) {
	    num = value;
	    rest = ({ });
	} else
	if (arrayp(value)) {
	    num = 1;
	    rest = value;
	} else
	    continue;

	oids = file->query_id();
	if( pointerp(oids) && sizeof(oids) )
	    name = oids[0];
	else if (stringp(oids))
	    name = oids;
	else
	    name = "";

	tally = sizeof(filter_array(inv, (: $1->id($(name)) :) ));
	num -= tally;
	for (int j = 0; j < num; j++) {
	    ret = new(file, rest...)->move(this_object(), "#CLONE#");
	    if ( ret != MOVE_OK )
	    {
		error("Initial clone failed for '" + file +"': " + ret + "\n");
	    }
	}
    }
}

//:FUNCTION set_objects
//Provide a list of objects to be loaded now and at every reset.  The key
//should be the filename of the object, and the value should be the number
//of objects to clone.  The value can also be an array, in which case the
//first element is the number of objects to clone, and the remaining elements
//are arguments that should be passed to create() when the objects are cloned.
//
//Note:  the number already present is determined by counting the number of
//objects with the same first id, and objects are only cloned to bring the
//count up to that number.
void set_objects(mapping m) {
    objects = m;
    reset();
}

//:FUNCTION can_take_from
//This is compat, I think

//### is this called?
int can_take_from() { return inventory_accessible(); }

//:FUNCTION can_put_in
//can objects put put into us?

//### this is outdated; should be done in indirect_put_obj_in_obj()
int can_put_in() {
    if (this_object()->query_closed()) {
        write(capitalize(the_short())+" is closed.\n");
        return -1;
    }
    return inventory_accessible();
}

//:FUNCTION introduce_contents
//returns a string appropriate for introduction the contents of an object
//in room descriptions.
string
introduce_contents() {
    switch (main_prep) {
    case "in": return capitalize(the_short()) + " contains:\n";
    case "on": return "Sitting on "+the_short()+" you see:\n";
    default: return capitalize(main_prep)+" "+the_short()+" you see:\n";
    }
}

varargs string inventory_recurse(int depth, mixed avoid) {
    string res;
    object *obs;
    int i;
    string str;
    string tmp;

    str = "";
    if(!arrayp(avoid))
      avoid = ({ avoid });
    obs = all_inventory(this_object()) - avoid;

    for (i=0; i<sizeof(obs); i++) {
        if (!(obs[i]->is_visible())) continue;
        if (!obs[i]->test_flag(TOUCHED) && obs[i]->untouched_long()) {
	    str += obs[i]->untouched_long()+"\n";
             if (obs[i]->inventory_visible())
                 str += obs[i]->inventory_recurse(depth+1, avoid);
        }
    }
    res = introduce_contents();
    if (tmp = inv_list(this_object(), 1, depth + 1, avoid)) {
      for (i=0; i<depth-1; i++) str += "  ";
      str += res + tmp;
    }
    return str;
}

string show_contents()
{
  return inventory_recurse();
}

//:FUNCTION set_preposition
//Sets what relation the contents of this object are to it.  I.e. are they
//in it, on it, etc ...
void set_preposition( string new_prep )
{
    if( origin() != ORIGIN_LOCAL ) return;

    main_prep = new_prep;
}

// Recognize this as a shitty scope hack?  I do.

string query_prep(){ return main_prep; }

// Right now this is called only if mass is set in an object that is in
//the env, or if release_object() has been called and recieve_object() bails
void update_capacity()
{
  mixed	masses;
  int m,i;

#ifdef USE_SIZE
  masses = all_inventory()->get_size();
#else
  masses = all_inventory()->query_mass();
#endif
  i = sizeof( masses );

  while( i-- )
    m += masses[i];

  set_capacity(m);
}

/* verb interaction */
mixed direct_get_obj(object ob, string name) {
    if (this_object() == environment(this_body())) {
	if (this_object()->get_item_desc(name))
	    return "That doesn't seem possible.\n";
        return "You're in it!\n";
    }
    return ::direct_get_obj(ob);
}

mixed indirect_put_obj_wrd_obj(object ob1, string prep, object ob2) {
    if (prep == main_prep)  {
        if(prep == "in")  {
            if (this_object()->query_closed()) {
                return capitalize(the_short())+" is closed.\n";
            } 
        }
    return 1;
    }                                                   

    switch (prep) {
	case "on":
	case "in":
	case "under":
	case "behind":
	    return "You can't put anything " + prep + " that.\n";
	default:
	    return 0;
    }
}

mixed direct_look_str_obj(string prep, object ob) {
    if (prep == main_prep)
	return 1;
    return "There is nothing " + prep + " " + the_short() + ".\n";
}

int contents_can_hear()
{
  return 1;
}

int internal_sounds_carry()
{
  return 1;
}

int environment_can_hear()
{
  object env = environment();
  return (internal_sounds_carry() && env && (!env->cant_hear_contents()));
}

// Inside messages propogate upward and downward...
void receive_inside_msg(string msg, object * exclude, int message_type, 
			mixed other)
{
  object env;
  object array contents;

  receive(msg);

  if(contents_can_hear())
    {
      contents = all_inventory(this_object());
      if(arrayp(exclude))
	contents -= exclude;
      contents->receive_outside_msg(msg, exclude, message_type, other);
    }
  if(environment_can_hear() && (env = environment()) && (!arrayp(exclude) || 
				   member_array(env, exclude) == -1))
    {
      env->receive_inside_msg(msg, arrayp(exclude) ? exclude + 
			      ({this_object()}) : ({this_object()}), 
			      message_type, other);
    }
}

// Outside messages propogate downward
void receive_outside_msg(string msg, object * exclude, int message_type,
			 mixed other)
{
  object array contents;

  receive(msg);

  if(contents_can_hear())
    {
      contents = all_inventory(this_object());
      if(arrayp(exclude))
	contents -= exclude;
      contents->receive_outside_msg(msg, exclude, message_type, other);
    }
}

//Remote messages propogate just like an inside message by defauly
void receive_remote_msg(string msg, object array exclude, int message_type,
			mixed other)
{
  receive_inside_msg(msg, exclude, message_type, other);
}

void receive_private_msg(string msg, int message_type, mixed other)
{
  receive(msg);
}

/* INTERNAL MUDLIB USAGE!!! NEVER CALL THIS!!! */
void containee_light_changed(int adjustment)
{
    contained_light += adjustment;

    /*
    ** if the containee is visible, then tweak our own light; this will
    ** propagate on up to the room
    */
    if ( inventory_visible() )
	adjust_light(adjustment);
}

void resync_visibility()
{
    int new_state;
    
    ::resync_visibility();

    new_state = inventory_visible();

    /* if it didn't change, jump ship */
    if ( new_state == contained_light_added )
	return;

    contained_light_added = new_state;

    if ( new_state )
	adjust_light(contained_light);
    else
	adjust_light(-contained_light);
}

int stat_me()
{
    write("Container capacity: "+query_capacity()+"/"+max_capacity+"\n");
    write("main_prep: " + main_prep + "\n");
    write("It contains:\n"+
      inv_list(this_object(), 0, 1)+"\n");
    return ::stat_me();
}
