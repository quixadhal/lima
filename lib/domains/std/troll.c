/* Do not remove the headers from this file! see /USAGE for more info. */

inherit MONSTER;

void create() {
    object sword;

    ::create();
    set_name("Bill");
    set_id( ({ "troll", "bill" }) );
    set_gender(1);
    set_proper_name("Bill the Troll");
    set_in_room_desc("Bill the Troll");
    set_long("Looking closely at a troll is something people usually try to avoid doing.");
    set_max_hp(10);

    sword = new(__DIR__ "sword");
    sword->move(this_object());
    do_wield(sword);
}
