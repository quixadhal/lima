/* Do not remove the headers from this file! see /USAGE for more info. */

#include "combat.h"

inherit SWORD;

mixed adjust_my_result(mixed res, string type) {
    if (res == "fatal") return "knockout";
    if (res == "dispatch") {
	environment()->targetted_action("$P $o seems unwilling to deal the killing blow, and veers to the side as $n $vtry to finish $t off.\n", environment()->query_target(), this_object());
	return -1;
    }
    return res;
}

create() {
    ::create();
    set_adj("glowing");
    set_proper_name("Sword of Mercy");
}
