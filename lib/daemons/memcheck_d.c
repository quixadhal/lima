/* Do not remove the headers from this file! see /USAGE for more info. */

string array known = ({ "Performing memory tests ..." });
string array last_post_array = ({});
int last_post;

void create() {
#ifndef __CHECK_MEMORY__
    destruct(this_object());
#else
    function f = function(function f) {
	string array s = explode(check_memory(), "\n") - known;

	if (sizeof(s - last_post_array) || sizeof(last_post_array - s)) {
	    if (last_post)
		NEWS_D->remove_post("driver.leaks", last_post);

	    if (sizeof(s))
		last_post = NEWS_D->system_post("driver.leaks", "check_memory() output", implode(s, "\n"), "MEMCHECK_D");
	}
	call_out(f, 3600, f);
    };
    
    call_out(f, 3600, f);
#endif
}
