/* Do not remove the headers from this file! see /USAGE for more info. */

/* Beek - 10/4/94 */
/* ported to Zork 1/15/95 */
#include <mudlib.h>
#include <security.h>

inherit CMD;

private mapping get_the_error(string str) {
    return unguarded(1, (: call_other, master(), "query_error", str :));
}

private void
main(string str) {
    mapping error;
    object ob;
    int i;

    if (!str || str == "") {
        error = get_the_error(this_user()->query_userid());
    } else {
        error = get_the_error(str);
	if (!error && !find_body(str) && str != "last") {
            out("No such player.\n");
            return;
        }
    }
    if (!error) {
        out("No error.\n");
    } else {
        outf("%s\n", master()->standard_trace(error, 1));
    }
    return;
}
