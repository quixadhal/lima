/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

int need_to_be_alive() {
    return 0;
}

int need_to_think() {
    return 0;
}

int need_to_see() {
    return 0;
}

mixed do_curse()
{
    write("Such language in a high class establishment like this!\n");
}

mixed * query_verb_info()
{
    return ({ ({ "" }), ({ "fuck","damn","shit","bitch"}) });
}
