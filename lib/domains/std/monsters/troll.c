/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ADVERSARY;
inherit M_BLOCKEXITS;
inherit M_ACTIONS;

void setup() {
   object sword;

    ::setup();
    set_name("Bill");
    set_id("troll", "bill");
    set_gender(1);
    set_proper_name("Bill the Troll");
    set_in_room_desc("Bill the Troll");
    set_long("Looking closely at a troll is something people usually try to avoid doing.");
    set_max_health(30);
   set_wielding("/domains/std/objects/sword");
}
