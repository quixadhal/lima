/* Do not remove the headers from this file! see /USAGE for more info. */
// complex_container.c

#include <move.h>
#include <setbit.h>
#include <driver/origin.h>
#include <hooks.h>

inherit CONTAINER;

private mapping obj_info = ([]);
private mapping relations = ([ ]);
private mapping capacities = ([]);
private mapping max_capacities = ([]);


void set_relations(string array...);


static int valid_prep(string prep) {
    return !!relations[prep];
}

varargs string query_prep(object ob) { return ob ? obj_info[ob] : ::query_prep(); }

varargs void set_max_capacity(int cap, string relation)
{
    if(!relation) relation = query_prep();
    if(!relations[relation])
	error("Invalid relation");
    max_capacities[relation] = cap;
}

varargs int query_max_capacity(string relation)
{
    if(!relation)
    {
	relation = query_prep();
    }
    return max_capacities[relation];
}

varargs void set_capacity (int cap, string relation)
{
    if(!relation) relation = query_prep();
    if(!sizeof(relations)) set_relations(query_prep());
    if(!relations[relation])
    {
	error("Invalid relation");
    }
    capacities[relation] = cap;
}

varargs int query_capacity(string relation)
{
    if(!relation)
    {
	relation = query_prep();
    }
    return capacities[relation];
}

//:FUNCTION set_relations
//Set the relations which are legal for a complex container.  Example:
//set_relations("on", "in", "under").  The first one is the default
//relation (the one used by set_objects(), etc)
void set_relations(string array rels...) {
    /* Ok, a bit tricky here.  We can't remove relations if there is an
       object occupying that, so we remove all relations with no objects,
       and add the new ones */
    foreach (string rel, object array obs in relations) {
	if (!sizeof(obs))
	    map_delete(relations, rel);
    }
    foreach (string str in rels)
    if (!relations[str])
	relations[str] = ({ });
}

string array get_relations()
{
    return keys(relations);
}


//:FUNCTION receive_object
//Determine whether we will accept having an object moved into us;
//returns a value from <move.h> if there is an error
mixed receive_object( object target, string relation )
{
    int x, m;

    if (!relation || relation == "#CLONE#")
	relation = query_prep();

    if( target == this_object() )
	return "You can't move an object inside itself.\n";

    /* Have to be a bit stricter here to keep relations[] sane */
    if (!relations[relation])
	return "You can't put things " + relation + " that.\n";

#ifdef USE_SIZE
    x = target->get_size();
#else
#ifdef USE_MASS
    x=target->query_mass();
#endif
#endif
    //    printf("cap=%O  mcap=%O  x=%O\n",query_capacity(),max_capacity,x);
    if ( (m=(query_capacity(relation))+x) > query_max_capacity(relation) )
    {
	return MOVE_NO_ROOM;
    }

    set_capacity( m, relation );
    //### if they back out later, capacity is fixed, but I don't remember how.
    //### Anyway, this needs fixing too.  There should be a uniform interface
    //### for that.  Basically just recalc capacity, and check that the objects
    //### in the relation arrays are actually inside the object.
    relations[relation] += ({ target });
    obj_info[target] = relation;

    return 1;
}

//:FUNCTION release_object
//Prepare for an object to be moved out of us; the object isn't allowed
//to leave if we return zero or a string (error message)
varargs mixed release_object( object target, int force )
{
    string relation = obj_info[target];

    relations[relation] -= ({target,0});
    map_delete(obj_info, target);
#ifdef USE_SIZE
    set_capacity(query_capacity(relation) - (target->get_size()), relation);
#else
#ifdef USE_MASS
    set_capacity(query_capacity(relation) - (target->query_mass()), relation);
#endif
#endif
    return 1;
}


void reinsert_object( object target, string relation )
{
#ifdef USE_SIZE
    set_capacity(query_capacity(relation) + (target->get_size()),
      relation);
#else
#ifdef USE_MASS
	set_capacity(query_capacity(relation) + (target->query_mass()),
	  relation);
#endif
#endif
    obj_info[target] = relation;
    relations[relation] = ({ target });

}


//:FUNCTION look_in
//returns a string containing the result of looking inside (or optionally
//a different relation) of the object
string look_in( string relation )
{
    string inv;
    mixed ex;

    if (!relation) relation = query_prep();

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

    if (!valid_prep(relation))
	return "There is nothing there.\n";

    inv = inv_list(relations[relation]);
    if ( !inv )
	inv = "  nothing";

    return (sprintf( "%s %s you see: \n%s\n",
	capitalize(query_prep()), short(),
	inv ));
}

string long()
{
    string res;
    string contents;

    res = simple_long();
    if (!inventory_visible()) return res;

    foreach (string rel, object array obs in relations) {
	contents = inv_list(obs, 1);
	if (contents)
	    res += introduce_contents(rel) + contents;
    }
    return res;
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

    foreach (object ob in obs) {
	if (!(ob->is_visible())) continue;
	if (!ob->test_flag(TOUCHED) && ob->untouched_long()) {
	    str += ob->untouched_long()+"\n";
	    if (ob->inventory_visible())
		str += ob->inventory_recurse(0, avoid);
	}
    }
    foreach (string rel, obs in relations) {
	res = introduce_contents(rel);
	if (tmp = inv_list(obs - avoid, 1, depth)) {
	    for (i=0; i<depth; i++) str += "  ";
	    str += res + tmp;
	}
    }
    return str;
}

mixed indirect_get_obj_from_obj(object ob1, object ob2) {
    if (relations["in"] && member_array(ob1, relations["in"]) != -1) {
	if (this_object()->query_closed()) {
	    return capitalize(the_short())+" is closed.\n";
	}
    }
    return 1;
}

string stat_me()
{
    return
    "Container capacity: "+query_capacity()+"/"+query_max_capacity()+"\n" +
    "Prepositions: " + implode(keys(relations), ",") + "\n" +
    "It contains:\n"+ show_contents() + "\n" +
    ::stat_me();
}

