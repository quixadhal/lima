/* Do not remove the headers from this file! see /USAGE for more info. */

/* Beek - 10/4/94 */
/* ported to Zork 1/15/95 */
#include <mudlib.h>
#include <security.h>

inherit CMD;

private void
main(string str) {
    mapping error;
    object ob;
    int i;

    if (!str || str == "") {
	error = master()->query_error(this_user()->query_userid());
    } else {
	error = master()->query_error(str);
	if (!error && !find_body(str) && str != "last") {
            write("No such player.\n");
            return;
        }
    }
    if (!error) {
        write("No error.\n");
    } else {
        printf("%s\n", master()->standard_trace(error, 1));
    }
    return;
}
