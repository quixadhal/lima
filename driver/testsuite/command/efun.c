#include <globals.h>

int
main(string fun)
{
    string leaks;
    object tp = this_player();
    
    if (!fun || fun == "") {
	string file;
	
	foreach (file in get_dir("/single/efuns/*.c")) 
	    main(file);
	write("Checks succeeded.\n");
	return 1;
    }
    ("/single/efuns/" + fun)->do_tests();
    if (tp != this_player())
	error("Bad this_player() after calling " + fun + "\n");
#if defined(DEBUGMALLOC_EXTENSIONS) && defined(CHECK_MEMORY)
    if (sizeof(explode(leaks = check_memory(), "\n")) != 1) {
	write("After calling: " + fun + "\n");
	write(leaks);
	error("LEAK\n");
    }
#endif
    return 1;
}
