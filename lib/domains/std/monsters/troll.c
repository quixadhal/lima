/* Do not remove the headers from this file! see /USAGE for more info. */

inherit MONSTER;
inherit M_BLOCKEXITS;

void setup() {
    object sword;

    set_name("Bill");
    set_id("troll", "bill");
    set_gender(1);
    set_proper_name("Bill the Troll");
    set_in_room_desc("Bill the Troll");
    set_long("Looking closely at a troll is something people usually try to avoid doing.");
    set_max_hp(10);

    add_block("north");
    set_block_action("$N1 $v1try to go $o, but $N $vblock $p1 path.\n");

    sword = new("/domains/std/objects/sword");
    sword->move(this_object());
    do_wield(sword);
}
