/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SWORD;
inherit M_VALUABLE;

void setup() {
    set_adj("dull");
    set_id("sword");
    set_weapon_class(15);
    set_size(MEDIUM);
    set_value(1000);
    add_combat_message("miss", "$N $vtake a clumsy swipe at $t, but only $vsucceed in making a fool of $r.\n");
}

