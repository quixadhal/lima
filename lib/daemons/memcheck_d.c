/* Do not remove the headers from this file! see /USAGE for more info. */

string array known = ({ "Performing memory tests ..." });

void create() {
#ifndef __CHECK_MEMORY__
    destruct(this_object());
#else
    function f = function(function f) {
	string array s = explode(check_memory(), "\n") - known;
	if (sizeof(s))
	    NEWS_D->system_post("driver.leaks", "check_memory() output", implode(s, "\n"));
	known += s;
	call_out(f, 3600, f);
    };
    
    call_out(f, 3600, f);
#endif
}
