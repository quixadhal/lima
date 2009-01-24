/* Do not remove the headers from this file! see /USAGE for more info. */

// container.c
//
// possibly the grossest piece of code in the mudlib.
// (not as bad as the code that describes Rust's mom, of course)
// Reworked a bit 2-17-99 by Tigran.  
// Reworking included merging CONTAINER and COMPLEX_CONTAINER

/* 
 * INCLUDES
 */
#include <move.h>
#include <setbit.h>
#include <driver/origin.h>
#include <hooks.h>
#include <clean_up.h>

/* 
 * INHERIT
 */
inherit OBJ;
inherit "/std/container/vsupport";

#ifdef SAY_HISTORY_IN_ROOMS
inherit "/std/body/history";
#endif 

/* 
 * CLASS DEFINITIONS
 */
class relation_data
{
  object array contents;           /* All of the objects associated with that relation */
  int max_capacity;                /* The maximum capacity of that relation. */
  int hidden;                      /* Whether the contents in this relation can be seen */
  int attached;                    /* Whether the contents of the relation will be added to 
                                      the relation's mass/size when calc'd */
  mixed hidden_func;               /* Function or int to be evaluated to determine visibility */
  mapping create_on_reset;         /* A mapping of filenames, quantities and parameters needed for
                                      objects to be updated as required every reset() in the relation */
  mapping create_unique_on_reset;  /* Mapping similar to that above, but the entire game is checked for the
                                      existance of the object before cloning */
}

/* 
 * VARIABLES
 */
private mapping relations = ([ ]);
private mapping relation_aliases=([]);/* Sometimes "in" and "on" etc mean the 
				       * same thing, for example on the bed. */
private string default_relation; /* The default relation used for this 
				       * container if none is specified.
				       * It should usually be "in" but 
				       * not necessarily */

int contained_light;
int contained_light_added;
mixed all_hidden_func;
varargs string introduce_contents(string relation);

/* 
 * PROTOTYPES
 */
string simple_long();
int inventory_visible();

/* 
 * FUNCTIONS
 */

/********    Relations    ********/

varargs void set_preposition(mixed args...){
    debug_message(base_name(this_object())+" says: \"RAWRR! I am a missing function! Fear me!\"");
}


/* Return 1 if the relation is valid, else 0 */
private nomask int valid_relation(string relation)
{
  relation=PREPOSITION_D->translate_preposition(relation);
  if(member_array(relation,keys(relations))==-1)
    return 0;
  return 1;
}

//:FUNCTION query_relation
//Return the relation which the given object is conatained by
string query_relation(object ob)
{
  foreach(string test,class relation_data values in relations)
    if(member_array(ob,values->contents)>-1)
      return test;
}

//:FUNCTION add_relation
//Add a relation to the complex container.
varargs void add_relation(string relation,int max_capacity,int hidden)
{
  class relation_data new_relation=new(class relation_data);
  relation=PREPOSITION_D->translate_preposition(relation);
  new_relation->max_capacity=max_capacity;
  new_relation->hidden=hidden;
  new_relation->contents=({});
  new_relation->create_on_reset=([]);
  new_relation->create_unique_on_reset=([]);
  relations[relation]=new_relation;
}

//:FUNCTION remove_relations
//Remove relations from an object.  Relations can only successfully be removed 
//if they are unoccupied.
void remove_relations(string array rels...)
{
  foreach(string rel in rels)
  {
    rel=PREPOSITION_D->translate_preposition(rel);
    if(sizeof(relations[rel]->contents))
      continue;
    map_delete(relations,rel);
  }
}

//:FUNCTION set_relations
//Set the relations which are legal for a complex container.  Example:
//set_relations("on", "in", "under").  The first one is the default
//relation (the one used by set_objects(), etc)
void set_relations(string array rels...)
{
/* Ok, a bit tricky here.  We can't remove relations if there is an
   object occupying that, so we remove all relations with no objects,
   and add the new ones */
/* Cheat and remove_relations() instead, it handles this already -- Tigran */
  remove_relations(keys(relations)...);
  foreach(string rel in rels)
  add_relation(rel,VERY_LARGE);
}

//:FUNCTION get_relations
//Return all of the possible relations for an object
string array get_relations(){ return keys(relations); }

//:FUNCTION is_relation_alias
//Determine whether or not the relation is valid and return which relation
//the alias is associated with.
string is_relation_alias(string test)
{
  foreach(string relation,mixed aliases in relation_aliases)
  {
    if(member_array(test,aliases)>-1)
      return relation;
  }
  return 0;
}

//:FUNCTION set_relation_alias
//Set the aliases that a relation has
void set_relation_alias(string relation,string aliases...)
{
  string aliased_to;
  relation=PREPOSITION_D->translate_preposition(relation);
  aliased_to=is_relation_alias(relation);
  if(!valid_relation(relation))
  {
    if(!aliased_to)
      error("Cannot set a relation alias to a nonexistant relation");
    relation=aliased_to;
  }
  relation_aliases[relation]=flatten_array(aliases);
}

//:FUNCTION add_relation_alias
//Add additional aliases that a relation has.
void add_relation_alias(string relation,string aliases...)
{
  string aliased_to;
  relation=PREPOSITION_D->translate_preposition(relation);
  aliased_to=is_relation_alias(relation);
  if(!valid_relation(relation))
  {
    if(!aliased_to)
      error("Cannot add a relation alias to a nonexistant relation");
    relation=aliased_to;
  }
  if(!sizeof(relation_aliases[relation]))
    set_relation_alias(relation,aliases...);
  else
    relation_aliases[relation]=flatten_array(relation_aliases[relation]+aliases);
}

//FUNCTION remove_relation_alias
//Remove aliases that a relation has.
void remove_relation_alias(string relation,string aliases...)
{
  relation_aliases[relation]-=aliases;
  if(!sizeof(relation_aliases))
    map_delete(relation_aliases,relation);
}

//:FUNCTION query_relation_aliases
//Return the array of aliases that a relation has.
string array query_relation_aliases(string relation)
{
  return relation_aliases[relation];
}

//:FUNCTION list_relation_aliases
//List all of the relation alias information
mapping list_relation_aliases()
{
  return relation_aliases;
}

//:FUNCTION set_default_relation
//Sets the default relation for the container.  This relation is used if no 
//relation is specified on many functions
void set_default_relation(string set)
{
  string aliased_to;
  set=PREPOSITION_D->translate_preposition(set);
  aliased_to=is_relation_alias(set);
  if(!valid_relation(set))
  {
    if(!aliased_to)
      error("Cannot set a nonexistant relation as default");
    default_relation=aliased_to;
  }
  default_relation=set;
}

//:FUNCTION query_default_relation
//Returns the default relation for the container.  See set_default_relation.
string query_default_relation(){ return default_relation; }


/********    Capacity   ********/


//:FUNCTION query_capacity
//Returns the amount of mass currently attached to a container
varargs int query_capacity(string relation)
{
  int cap;
  string aliased_to;
/* Need a little special handling for #CLONE# */
  if(!relation||relation==""||relation=="#CLONE#")
    relation=query_default_relation();
  relation=PREPOSITION_D->translate_preposition(relation);
  aliased_to=is_relation_alias(relation);
  if(!valid_relation(relation))
  {
    if(!aliased_to)
      return 0;
    relation=aliased_to;
  }
  foreach(object ob in relations[relation]->contents)
  {
    if(!ob)
    {
      relations[relation]->contents-=({ob});
      continue;
    }
#ifdef USE_SIZE 
    cap+=ob->query_size();
#else
    cap+=ob->query_mass();
#endif
  }
  return cap;
}

//:FUNCTION set_max_capacity
//Set the maximum capacity for a given relation.
varargs void set_max_capacity(int cap, string relation)
{
    string aliased_to;
    if(!relation||relation=="")
	relation=query_default_relation();
    relation=PREPOSITION_D->translate_preposition(relation);
    aliased_to=is_relation_alias(relation);
    if(!valid_relation(relation))
    {
	if(!aliased_to)
	    error("Invalid relation");
	relation=aliased_to;
    }
    relations[relation]->max_capacity=cap;
}

//:FUNCTION query_max_capacity
//Returns the maximum capacity for a given relation
varargs int query_max_capacity(string relation)
{
    string aliased_to;
    if(!relation||relation=="")
	relation=query_default_relation();
    relation=PREPOSITION_D->translate_preposition(relation);
    aliased_to=is_relation_alias(relation);
    if(!valid_relation(relation))
    {
	if(!aliased_to)
	    return 0;
	relation=aliased_to;
    }
    return relations[relation]->max_capacity;
}

//### Yo! finish this -- Tigran 
//:FUNCTION query_total_capacity
//Returns the capacity directly attributed to the container.  This should 
//normally include anything attached or within the container.
int query_total_capacity()
{

}
#ifdef USE_MASS
//:FUNCTION query_mass
int query_mass()
{
    return query_total_capacity() + ::query_mass();
}
#endif

#ifdef USE_SIZE
int query_aggregate_size()
{
    return query_total_capacity() + ::query_size();
}
#endif


//:FUNCTION receive_object
//Determine whether we will accept having an object moved into us;
//returns a value from <move.h> if there is an error
mixed receive_object( object target, string relation )
{
    int x, m;
    string aliased_to;
    if(!relation||relation==""||relation=="#CLONE#")
	relation=query_default_relation();
    relation=PREPOSITION_D->translate_preposition(relation);
    aliased_to=is_relation_alias(relation);
    if( target == this_object() )
	return "You can't move an object inside itself.\n";

    /* Have to be a bit stricter here to keep relations[] sane */
    if (!valid_relation(relation))
    {
	if(!aliased_to)
	    return "You can't put things " + relation + " that.\n";
	relation=aliased_to;
    }


#ifdef USE_SIZE
    x = target->query_size();
#else
    x=target->query_mass();
#endif

    if ( (m=(query_capacity(relation))+x) > query_max_capacity(relation) )
    {
	return MOVE_NO_ROOM;
    }
    relations[relation]->contents += ({ target });
    return 1;
}

//:FUNCTION release_object
//Prepare for an object to be moved out of us; the object isn't allowed
//to leave if we return zero or a string (error message)
varargs mixed release_object( object target, int force )
{
    string relation;
    if(!target||force)
	return 1;
    relation=query_relation(target);
    if(!relation&&!force)
	return 0;
    relations[relation]->contents-=({target});
    return 1;
}


void reinsert_object( object target, string relation )
{
    if(!relation)
	relation = query_default_relation();
    relations[relation]->contents += ({ target });
}


/********   Descriptions    ********/

string long()
{
  string res;
  string contents;

  res = simple_long();
  if (!inventory_visible())
    return res;

  foreach (string rel,class relation_data data in relations)
  {
    contents = inv_list(data->contents, 1);
    if (contents)
    {
	    res += introduce_contents(rel) + contents;
    }
  }
  return res;
}

//:FUNCTION look_in
//returns a string containing the result of looking inside (or optionally
//a different relation) of the object
string look_in( string relation )
{
    string inv;
    mixed ex;
    string aliased_to;
    if(!relation||relation=="")
	relation=query_default_relation();
    relation=PREPOSITION_D->translate_preposition(relation);
    aliased_to=is_relation_alias(relation);
    //    if (!relation) relation = query_prep();

    //:HOOK prevent_look
    //A set of yes/no/error hooks which can prevent looking <relation> OBJ
    //The actual hook is prevent_look_<relation>, so to prevent looking
    //in something use prevent_look_in.
    ex = call_hooks("prevent_look_" + relation, HOOK_YES_NO_ERROR);
    if(!ex)
	ex = call_hooks("prevent_look_all", HOOK_YES_NO_ERROR);
    if (!ex) ex = "That doesn't seem possible.";
    if (stringp(ex))
	return ex;

    if (!valid_relation(relation))
    {
	if(!aliased_to)
	    return "There is nothing there.\n";
	relation=aliased_to;
    }

    inv = inv_list(relations[relation]->contents);
    if ( !inv )
	inv = "  nothing";

    return (sprintf( "%s %s you see: \n%s\n",
	capitalize(relation),
	short(),
	inv ));
}


varargs void set_hide_contents( mixed hide, string relation )
{
    string aliased_to;
    if(hide)
    {
	if(!relation)
	{
	    all_hidden_func=hide;
	    add_hook( "prevent_look_all", all_hidden_func );
	}
	else
	{
	    relation=PREPOSITION_D->translate_preposition(relation);
	    aliased_to=is_relation_alias(relation);
	    if(!valid_relation(relation))
	    {
		if(!aliased_to)
		    error("Attempted to hide contents of a container with a "
		      "nonexistant relation.");
		relation=aliased_to;
	    }
	    relations[relation]->hidden_func=hide;
	    add_hook ("prevent_look_"+relation,relations[relation]->hidden_func);
	}
    }
    else 
    {
	if(!relation)
	{
	    remove_hook( "prevent_look_all", all_hidden_func );
	    all_hidden_func=0;
	}
	relation=PREPOSITION_D->translate_preposition(relation);
	if(!valid_relation(relation))
	{
	    if(!aliased_to)
		error("Attempted to unhide contents of a container with a nonexistant "
		  "relation");
	    relation=aliased_to;
	}
	remove_hook( "prevent_look_"+relation, relations[relation]->hidden_func );
	relations[relation]->hidden_func=0;
    }   
}

mixed query_hide_contents(string relation)
{
    string aliased_to;
    if(!relation)
	return all_hidden_func;
    relation=PREPOSITION_D->translate_preposition(relation);
    aliased_to=is_relation_alias(relation);
    if(!valid_relation(relation))
    {
	if(!aliased_to)
	    return 0;
	relation=aliased_to;
    }
    return relations[relation]->hidden_func;
}


//:FUNCTION simple_long
//Return the long description without the inventory list.
string simple_long() {
    return ::long();
}


//:FUNCTION ob_state
//Determine whether an object should be grouped with other objects of the
//same kind as it.  -1 is unique, otherwise if objects will be grouped
//according to the return value of the function.
mixed ob_state() {
    /* if we have an inventory, and it can be seen, we should be unique */
    if (first_inventory(this_object()) && inventory_visible()) return -1;
    //### hack
    if (this_object()->query_closed()) return "#closed#";
    return ::ob_state();
}


//:FUNCTION parent_environment_accessible
//Return 1 if the parser should include the outside world in its
//decisions, overloaded in non_room descendants
int parent_environment_accessible() {
    return 0;
}


/********    Inventory    ********/


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

/* Function which creates objects on reset if they are needed. */

mixed array make_objects_if_needed()
{
    mixed *objs = ({});

    /* Start by looping through each of the relations of the object */
    foreach(string relation,class relation_data object_data in relations)
    {
	/* Loop through each element of the mapping */
	object_data->contents-=({0});
	foreach(string file,mixed parameters in object_data->create_on_reset)
	{
	    int num=1;
	    mixed array rest=({});
	    object array matches=({});
	    /* Allow use of relative paths, relative to the container. */
	    file = absolute_path(file, this_object());
	    /* If the only parameter is an integer, it is the quantity of the
	     * object that needs to be in this relation */
	    if (intp(parameters)) 
		num = parameters;
	    else
	    if (arrayp(parameters)) 
	    {
		/* Check the first argument for an integer value, if it is
		 * then it is the quantity fo the object to be in the 
		 * relation */
		if(intp(parameters[0]))
		{
		    num = parameters[0];
		    rest = parameters[1..];
		}
		/* Everything else is parameters passed to create() */
		else
		    rest = parameters;
	    }
	    else
		continue;
	    if(sizeof(object_data->contents))
	    {
		matches=filter(object_data->contents,(:cannonical_form ($1)==$(file) :) );
	    }
	    while(sizeof(matches)<num)
	    {
		int ret;
		object ob = new(absolute_path(file), rest...);
		if(!ob)
		    error("Couldn't find file '" + file + "' to clone!\n");
		ret = ob->move(this_object(), "#CLONE#");
		if ( ret != MOVE_OK )
		    error("Initial clone failed for '" + file +"': " + ret + "\n");
		ob->on_clone( rest... );
		matches+=({ob});
	    }
	    objs+=matches;
	}
    }
    return objs;
}

mixed array make_unique_objects_if_needed()
{
    mixed array objs=({});

    /* Loop through each relation */
    foreach(string relation,class relation_data object_data in relations)
    {
	/* Loop through each file in the mapping. */
	foreach (string file, mixed parameters in relations[relation]->create_unique_on_reset)
	{
	    mixed array rest=({});
	    int num;
	    object array matches=({});
	    /* Allow use of relative paths, relative to the container. */
	    file = absolute_path(file, this_object());
	    /* If the only parameter is an integer, it is the quantity of the
	     * object that needs to be in this relation */
	    if (intp(parameters)) 
		num = parameters;
	    else
	    if (arrayp(parameters)) 
	    {
		/* Check the first argument for an integer value, if it is
		 * then it is the quantity for the object to be in the 
		 * relation */
		if(intp(parameters[0]))
		{
		    num = parameters[0];
		    rest = parameters[1..];
		}
		/* Everything else is parameters passed to create() */
		else
		    rest = parameters;
	    }
	    else
		continue;
	    matches=children(file);
	    matches = filter(matches, (: clonep($1) :));
	    /* Clone x of the object to catch it up to the number of objects 
	     * requested by the mapping */
	    while(sizeof(matches)<num)
	    {
		int ret;
		object ob = new(absolute_path(file), rest...);
		if(!ob)
		    error("Couldn't find file '" + file + "' to clone!\n");
		/* Test for uniqueness in the object by calling test_unique() */
		if(ob->test_unique())
		    break;
		ret = ob->move(this_object(), "#CLONE#");
		if ( ret != MOVE_OK )
		    error("Initial clone failed for '" + file +"': " + ret + "\n");
		ob->on_clone( rest... );
		matches+=({ob});
	    }
	    objs+=matches;
	}
    }
    return objs;
}

//:FUNCTION set_objects
//Provide a list of objects to be loaded now and at every reset.  The key
//should be the filename of the object, and the value should be the number
//of objects to clone.  The value can also be an array, in which case the
//first element is the number of objects to clone, and the remaining elements
//are arguments that should be passed to create() when the objects are cloned.
//An optional second string argument represents a specific relation which
//should produce objects on reset()
//
//Note:  the number already present is determined by counting the number of
//objects with the same first id, and objects are only cloned to bring the
//count up to that number.
varargs mixed array set_objects(mapping m,string relation) {
    if(!relation||relation=="")
	relation=query_default_relation();
    relation=PREPOSITION_D->translate_preposition(relation);
    relations[relation]->create_on_reset = m;
    return make_objects_if_needed();
}

//:FUNCTION set_unique_objects
//Provide a list of objects to be loaded now and at every reset if they
//are not already loaded.  The key should be the filename of the object, 
//and the value should be an array which is passed to create() when the 
//objects are cloned.
//The structure of the mapping should be the same as the structure of the 
//mapping for set_objects().  For unique objects, to be checked, you should
//have a function in the object called test_unique() which will return 1 if 
//uniqueness requirements are met.  The prototype for the function is
//        int test_unique();
//An optional second string argument represents a specific relation which
//should produce objects on reset()
varargs mixed array set_unique_objects(mapping m,string relation) {
    if(!relation||relation=="")
	relation=query_default_relation();
    relation=PREPOSITION_D->translate_preposition(relation);
    relations[relation]->create_unique_on_reset = m;
    return make_unique_objects_if_needed();
}

/* Hrm, are the following two functions really necessary?  Should the relation
 * themselves have more say in this? */

//:FUNCTION introduce_contents
//returns a string appropriate for introduction the contents of an object
//in room descriptions.
varargs string introduce_contents(string relation)
{
  if(!relation||relation=="")
    relation=query_default_relation();
  relation=PREPOSITION_D->translate_preposition(relation);
  switch (relation)
  {
    case "in":
      return capitalize(the_short()) + " contains:\n";
    case "on":
      return "Sitting on "+the_short()+" you see:\n";
    default:
      return capitalize(relation)+" "+the_short()+" you see:\n";
  }
}

/* //:FUNCTION inventory_header */
/* //Returns a string header to put before inventory lists */
/* string inventory_header() { */
/*     switch (main_prep) { */
/*     case "in": return "It contains:\n"; */
/*     case "on": return "Sitting on it you see:\n"; */
/*     default: return capitalize(main_prep)+" it you see:\n"; */
/*     } */
/* } */

varargs string inventory_recurse(int depth, mixed avoid)
{
  string res;
  object array obs;
  int i;
  string str="";
  string tmp;

  if (avoid) 
  {
    if(!arrayp(avoid))
      avoid = ({ avoid });
  } else
    avoid = ({});


  if (!this_object()->is_living())
  {
    obs = all_inventory() - avoid;
    foreach (object ob in obs)
    {
      if (!(ob->is_visible()))
        continue;
	    if (!ob->test_flag(TOUCHED) && ob->untouched_long())
	    {
        str += ob->untouched_long()+"\n";
        if (ob->inventory_visible())
          if (!ob->is_living())
            str += ob->inventory_recurse(0, avoid);
      }
    }
  }
  if (!this_object()->is_living())
  {
    foreach(string key,mixed data in relations)
    {
	    res = introduce_contents(key);
	    tmp = inv_list(data->contents-avoid, 1, depth);
      if (tmp)
      {
        for (i=0; i<depth; i++)
          str += "  ";
        str += res + tmp;
      }
    }
  }
  return str;
}

string show_contents()
{
    return inventory_recurse();
}

//:FUNCTION inventory_accessible
//Return 1 if the contents of this object can be touched, manipulated, etc
int inventory_accessible() {
    return 1;
    /*     if (!is_visible()) return 0; */
    /*     if (!short()) return 0; */
    /*     return !this_object()->query_closed(); */
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

// We use this so that bodies can overload this
void do_receive(string msg, int msg_type) {
    receive(msg);
}

// Inside messages propogate upward and downward...
varargs void
receive_inside_msg(string msg, object * exclude, int message_type, 
  mixed other)
{
    object env;
    object array contents;

    do_receive(msg, message_type);

    /* downwards (into our contents) */
    if(contents_can_hear())
    {
	contents = all_inventory(this_object());
	if(arrayp(exclude))
	    contents -= exclude;
	contents->receive_outside_msg(msg, exclude, message_type, other);
    }

    /* upwards (to our environment) */
    if(environment_can_hear() && (env = environment()) && (!arrayp(exclude) || 
	member_array(env, exclude) == -1))
    {
	env->receive_inside_msg(msg, arrayp(exclude) ? exclude + 
	  ({this_object()}) : ({this_object()}), 
	  message_type, other);
    }
}

// Outside messages propogate downward (into our contents)
varargs void
receive_outside_msg(string msg, object * exclude, int message_type,
  mixed other)
{
    object array contents;

    do_receive(msg, message_type);

    if(contents_can_hear())
    {
	contents = all_inventory(this_object());
	if(arrayp(exclude))
	    contents -= exclude;
	contents->receive_outside_msg(msg, exclude, message_type, other);
    }
}

//Remote messages propogate just like an inside message by defauly
varargs void 
receive_remote_msg(string msg, object array exclude, int message_type,
  mixed other)
{
    receive_inside_msg(msg, exclude, message_type, other);
}

varargs void
receive_private_msg(string msg, int message_type, mixed other)
{
    do_receive(msg, message_type);
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


// If this is called, clean_up in /std/object has decided we might be
// useless.  Make sure we don't have any 'people' inside us, though.
int destruct_if_useless() {
    foreach (object ob in deep_inventory(this_object())) {
	object link = ob->query_link();

	if (link && userp(link))
	    return ASK_AGAIN;
    }
    return ::destruct_if_useless();
}

mapping lpscript_attributes()
{
  return ([
    "objects" : ({ LPSCRIPT_OBJECTS }),
    "capacity" : ({ LPSCRIPT_INT, "setup", "set_max_capacity" }),
    "relations" : ({ LPSCRIPT_LIST, "setup", "set_relations" }),
    "default_relation" : ({ LPSCRIPT_STRING, "setup", "set_default_relation" }),
    ]);
}


/********    Miscellaneous    ********/

//:FUNCTION is_container
//Returns 1 if an object is a container
int is_container()  { return 1; }


string stat_me()
{
    return
    "Prepositions: " + implode(keys(relations), ",") + "\n" +
    "It contains:\n"+ show_contents() + "\n" +
    ::stat_me();
}

void reset()
{
    make_objects_if_needed();
    make_unique_objects_if_needed();
}
