/* Do not remove the headers from this file! see /USAGE for more info. */

#include "mudlib.h"

inherit SWORD;

create() {
    ::create();
    add_adj("dull");
    set_weapon_class(15);
    // override the default messages for 'miss'
    add
}

