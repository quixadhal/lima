/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

private void main(string str) {
    mixed data;
    mixed *m;
    int i;

    if (!str) {
	write("showemote verb\n");
	return;
    }
    data = SOUL_D->query_emote(str);
    if (!data) {
        write("No such emote.\n");
        return;
    }
    m = keys(data);
    for (i=0; i<sizeof(m); i++) {
        if (stringp(data[m[i]])) {
            printf("%O -> %s\n", m[i], data[m[i]]);
        } else {
	    printf("%O -> %s\n", m[i], implode(data[m[i]], "\n\t-> "));
        }
    }
}

