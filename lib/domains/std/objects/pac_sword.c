/* Do not remove the headers from this file! see /USAGE for more info. */

#include "old/combat.h"

inherit SWORD;
inherit CLASS_COMBAT_RESULT;

class combat_result array
adjust_my_result(class combat_result array result) {
    foreach (class combat_result res in result) {
	if (res->special & (RES_FATAL | RES_NO_RESISTANCE)) {
	    res->special = 0;
	    res->damage = 0;
	    res->message = "$P $o seems unwilling to deal the killing blow, and veers to the side as $n $vtry to finish $t off.\n";
	}
	if (res->special & RES_MIGHT_BE_FATAL) {
	    res->damage = 0;
	    res->special = RES_KNOCKDOWN;
	}
	if (res->special & RES_FATAL) {
	    res->damage = 0;
	    res->special = RES_KNOCKOUT;
	}
    }
    return result;
}

void setup() {
    set_adj("glowing");
    set_id("sword");
    add_id( "mercy" );
    add_adj( "sword of" );
    set_proper_name("the Sword of Mercy");
    set_size(MEDIUM);
}
