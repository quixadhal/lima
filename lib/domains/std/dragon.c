/* Do not remove the headers from this file! see /USAGE for more info. */

inherit MONSTER;
inherit M_BLOCKEXITS;
inherit M_WANDER;

void setup() {
    object sword;

    set_name("Tiamat");
    set_id("troll", "tiamat");
    set_gender(1);
    set_proper_name("Tiamat");
    set_in_room_desc("Tiamat");
    set_long("Getting close enough to see what a dragon is not a good idea.");

    set_max_hp(300);
  set_movement_time(5);
set_wander_area("wiz_area");

}

