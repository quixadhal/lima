/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

mixed can_curse() /* ability to, not permission to :) */
{
    return 1;
}

mixed do_curse()
{
    write("Such language in a high class establishment like this!\n");
}

mixed * query_verb_info()
{
    return ({ ({ "" }), ({ "fuck","damn","shit","bitch"}) });
}
