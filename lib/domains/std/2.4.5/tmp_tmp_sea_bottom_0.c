/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/monster";

void setup();

void setup() {
function f;
set_proper_name("octopus");
set_max_hp(100);
set_weapon_class(12);
set_in_room_desc("An octopus");
set_long("A very big octopus with long arms, reaching for you.");
set_max_capacity(50);
set_objects( ([
  "obj/chest.scr" : ({ random(500) }),
]) );
}

