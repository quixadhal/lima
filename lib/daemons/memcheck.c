/* Do not remove the headers from this file! see /USAGE for more info. */

string last;

void create() {
#ifndef __CHECK_MEMORY__
    destruct(this_object());
#else
    function f = function(function f) {
	string s = check_memory();
	s = replace_string(s, "Performing memory tests ...\n", "");
	if (strlen(s))
	    NEWS_D->system_post("driver.leaks", "check_memory() output", s);
	call_out(f, 3600, f);
    };
    
    evaluate(f);
#endif
}
