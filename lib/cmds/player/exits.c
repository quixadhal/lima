/* Do not remove the headers from this file! see /USAGE for more info. */

// _exits.c
// written by:  Rust
// Megaboz@ZorkMUD edited help and attached header 5-5-94

#include <mudlib.h>


int main()
{
    mapping exits;
    string* files;
    string* directions;
    string Short;
    int i;
    object o;

    exits = environment(this_body())->get_exits();
    if(!exits){
	write("There are no exits to this room.\n");
	return 0;
    }

    files = values( exits );
    directions = keys( exits );

    if( !sizeof( files ) ){
	write("There are no exits in this room.\n");
	return 0;
    }

    write("Current exits:\n");

    i = sizeof(files);
    while(i--){
	if( !o = load_object(files[i]) )
	    Short = "Under construction";
	else    Short = o->short();
	if(!Short) Short = "";
	printf("%s:  %s \n",
	       directions[i], Short);
    }
    return 1;
}
