/* Do not remove the headers from this file! see /USAGE for more info. */

#include <verbs.h>

inherit NVERB_OB;

mixed do_curse()
{
    write("Such language in a high class establishment like this!\n");
}

void create()
{
    add_rules( ({ "" }), ({ "fuck","damn","shit","bitch"}) );
    clear_flag(NEED_TO_SEE);
    clear_flag(NEED_TO_THINK);
    clear_flag(NEED_TO_BE_ALIVE);
}
