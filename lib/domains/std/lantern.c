/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_SWITCHABLE;
inherit M_LIGHTABLE;

void create() {
    switchable::create();
    ::create();
    set_id( ({ "lantern" }) );
    set_adjectives( ({ "brass" }) );
    set_gettable(1);
    set_long("Somehow, this brass lantern looks vaguely familiar.\n");

    set_fuel(5);
	
    add_hook("prevent_turn_on", (: check_fuel :));
    add_hook("extinguish", (: set_is_on, 0 :));
    add_hook("turn_on", (: light :));
    add_hook("turn_off", (: extinguish :));
 }
