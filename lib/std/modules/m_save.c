/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Inheritable for persistance.
** Beek for nightmare, ported by Rust.
** could use helper funs that save directly to 
** a file, and some security (when programatic security arrives)
** save() and restore() or save_to_file(), etc....
**
** Beek - static'ed a bunch of things which shouldn't be called externally
*/

#include <mudlib.h>

//:MODULE
//This module implements persistence

private static int save_recurse;
private static mixed *saved = ({});

//###These filenames can legally be remapped to other objects (these moved)
private static string * old_fnames = ({ "/obj/shells/wish", "/obj/pshell" });

//:FUNCTION add_save
//Mark a variable as one that gets saved.
static void add_save(mixed *vars) { 
    saved += vars;
//  saved = clean_array(saved + vars);
}

//:FUNCTION get_saved
//returns the array of variables that get saved.

//###Security problem here - Beek.  What is it used for anyway?
string *get_saved() { return saved; }

//:FUNCTION set_save_recurse
//sets whether or not a save is recursive.
static void set_save_recurse(int flag) {  save_recurse = flag; }

//:FUNCTION save_to_string
//saves an object into a string representation.
varargs string save_to_string(int recursep) {
    string *tmpsaved;
    string *values;
    string ret;
    string var;
    mapping map = ([]);

    if (recursep)
      set_save_recurse (1);

    tmpsaved  = decompose(map(saved, (: functionp($1) ? evaluate($1, "saving") : $1 :)));
    tmpsaved -= ({0});
    foreach (var in tmpsaved)
	map[var] = fetch_variable(var);

    map["#base_name#"] = base_name(this_object());
    if (save_recurse)
	map["#inventory#"] = all_inventory()->save_to_string() - ({ 0 });

    return save_variable(map);
}

//:FUNCTION load_from_string
//loads an object from a string representation.

//### obsolete code
void old_load_from_string(mixed value, int recurse) {
    string obj;
    object ob;
    int i;
    string str;
    string *tmpsaved;

    if(!stringp(value))
      return;
    value = restore_variable(value);
    if (value[0] != base_name(this_object()))
    {
	/*
	** If the saved fname is not a "legal" old name, then raise an
	** error.  There are certain files that have moved, so we must
	** deal appropriately with errors.
	*/
	if ( member_array(value[0], old_fnames) == -1 )
	    error("Invalid save string (ob)\n");
    }

    tmpsaved = decompose(map(saved, (: functionp($1) ? evaluate($1, "loading") : $1 :)));
    tmpsaved -= ({0});
    if (sizeof(tmpsaved) != sizeof(value[1]))
	error("Invalid save string (size)\n");
    for (i = 0; i < sizeof(tmpsaved); i++)
	store_variable(tmpsaved[i], value[1][i]);
    if (sizeof(value) == 3) {
	foreach(obj in value[2]) {
	    value = restore_variable(obj);
	    ob = new(value[0]);
	    ob->load_from_string(value, 1);
	}
    }
    if (recurse) move_object(previous_object());
}

void load_from_string(mixed value, int recurse) {
    mixed data;
    mixed *tmpsaved;
    string var;
    mixed val;
    string obj;
    object ob;
    
    if (!mapp (value))
      data = restore_variable(value);
    else
      data = value;
    if (arrayp(data)) {
	old_load_from_string(value, recurse);
	return;
    }
    
    if (data["#base_name#"] != base_name(this_object()))
    {
	/*
	** If the saved fname is not a "legal" old name, then raise an
	** error.  There are certain files that have moved, so we must
	** deal appropriately with errors.
	*/
	if ( member_array(data["#base_name#"], old_fnames) == -1 )
	    error("Invalid save string (ob)\n");
    }

    tmpsaved = decompose(map(saved, (: functionp($1) ? evaluate($1, "loading") : $1 :)));
    tmpsaved -= ({0});

    foreach (var, val in data) {
	if (var[0] == '#') continue;
	store_variable(var, val);
    }
    if (data["#inventory#"]) {
      int c;
	foreach(obj in data["#inventory#"]) {
	    val = restore_variable(obj);
	    if(!(ob = new(val["#base_name#"])))
	      continue;
	    ob->load_from_string(val, recurse+1); 
	}
    }
    // We don't want to move the object we're restoring into the
    // previous_object ()  (happens at the end)...  When you restored the body,
    // the body moved into the user object, but couldn't move out! -- Rust

    if (recurse > 1) 
      {
	move_object(previous_object());
      }
}

