/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ARMOR;

mixed adjust_result(mixed result, string type, object attacker) {
    if (result == "miss" || result == "miss" || result == -1) return result;
    if (random(5)) {
	attacker->targetted_action("$P blows glance off $p1 $o, leaving $t completely unharmed!\n", environment(), this_object());
	return -1;
    }
    return result;
}

create() {
    ::create();
    add_id("platemail");
    add_adj("admantine");
    // platemail acts as if it were plural
    set_proper_name("admantine platemail");
    // would call set_armor_class() here, but it isn't really necessary
    // since we are overloading adjust_result()
}
