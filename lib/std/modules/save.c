/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Inheritable for persistance.
** Beek for nightmare, ported by Rust.
** could use helper funs that save directly to 
** a file, and some security (when programatic security arrives)
** save() and restore() or save_to_file(), etc....
**
*/

#include <mudlib.h>
DOC_MODULE("implements persistance.")

private int save_recurse;
private mixed *saved = ({});

DOC(add_save, "mark a variable as one that gets saved.")
void add_save(mixed *vars) { 
  saved = clean_array(saved + vars);
}

DOC(get_saved, "returns the array of variables that get saved.")
string *get_saved() { return saved; }

DOC(set_save_recurse, "sets whether or not a save is recursive.")
void set_save_recurse(int flag) {  save_recurse = flag; }

DOC(save_to_string, "saves an object into a string representation.")
string save_to_string() {
    string *tmpsaved;
    string *values;
    string ret;
    tmpsaved  = decompose(map(saved, (: functionp($1) ? evaluate($1, "saving") : $1 :)));
    tmpsaved -= ({0});
    values = map(tmpsaved, (: fetch_variable :) );
    if( !save_recurse ) 
      return save_variable(({ base_name(this_object()), values }));
    return save_variable(({ base_name(this_object()), values,
				all_inventory()->save_to_string() - ({ 0 })
				}));
}

DOC(load_from_string, "loads an object from a string representation.")
void load_from_string(mixed value, int recurse) {
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
