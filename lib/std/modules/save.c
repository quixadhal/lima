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
DOC_MODULE("implements persistance.")

private int save_recurse;
private mixed *saved = ({});

DOC(add_save, "mark a variable as one that gets saved.")
static void add_save(mixed *vars) { 
    saved += vars;
//  saved = clean_array(saved + vars);
}

DOC(get_saved, "returns the array of variables that get saved.")
// Security problem here - Beek
string *get_saved() { return saved; }

DOC(set_save_recurse, "sets whether or not a save is recursive.")
static void set_save_recurse(int flag) {  save_recurse = flag; }

DOC(save_to_string, "saves an object into a string representation.")
string save_to_string() {
    string *tmpsaved;
    string *values;
    string ret;
    string var;
    mapping map = ([]);

    tmpsaved  = decompose(map(saved, (: functionp($1) ? evaluate($1, "saving") : $1 :)));
    tmpsaved -= ({0});
    foreach (var in tmpsaved)
	map[var] = fetch_variable(var);

    map["#base_name#"] = base_name(this_object());
    if (save_recurse)
	map["#inventory#"] = all_inventory()->save_to_string() - ({ 0 });

    return save_variable(map);
}

DOC(load_from_string, "loads an object from a string representation.")
// ### obsolete code
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
	error("Invalid save string (ob)\n");
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
    
    data = restore_variable(value);
    if (arrayp(data)) {
	old_load_from_string(value, recurse);
	return;
    }
    
    if (data["#base_name#"] != base_name(this_object()))
	error("Invalid save string (ob)\n");
    tmpsaved = decompose(map(saved, (: functionp($1) ? evaluate($1, "loading") : $1 :)));
    tmpsaved -= ({0});

    foreach (var, val in data) {
	if (var[0] == '#') continue;
	store_variable(var, val);
    }
    if (data["#inventory#"]) {
	foreach(obj in data["#inventory#"]) {
	    val = restore_variable(obj);
	    ob = new(val["#base_name#"]);
	    ob->load_from_string(val, 1);
	}
    }
    if (recurse) move_object(previous_object());
}

