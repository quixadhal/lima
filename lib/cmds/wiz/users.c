/* Do not remove the headers from this file! see /USAGE for more info. */

/* List the user names of all people listed in the "users()" array
 *
 * Peregrin@ZorkMUD
 */

#include <mudlib.h>




int main()
{
    object* ulist;
    string ustring;

    ulist = users();
    if (sizeof(ulist) <= 0) {
	write(sprintf("NO ONE LOGGED IN!!!   (not even you!)\n"));
	return 1;
    }
    if ((sizeof(ulist)==1) && (ulist[0]==this_user())) {
	write(sprintf("You are the only person logged in!\n"));
	return 1;
    }
    write(wrap(implode(ulist->query_userid(), " ") + "\n"));

    return 1;
}
