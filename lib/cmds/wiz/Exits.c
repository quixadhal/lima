/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>


int main(){

    mapping exits;
    string* files;
    string* directions;
    string Short;
    int i;
    object o;
    string base;

    exits = environment(this_body())->get_exits();
    base = environment(this_body())->query_base();
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
	if(stringp(files[i]))
	{
            if (!(o = load_object(files[i])) && !(o = load_object(evaluate_path(base + files[i]))))
		Short = "Under construction";
	    else    Short = o->short();
	    if(!Short) Short = "";
	    printf("%s:  %s (%s)\n",
	      directions[i], Short, files[i]);
	}
	else printf("%s:  Unknown\n",directions[i]);
    }
    return 1;
}

int help(){
    write("Usage:\n"
      "The exits command shows what it says.\n");
    return 1;
}
