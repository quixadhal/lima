/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WANDERING_MONSTER;
inherit M_BLOCKEXITS;

void setup()
{
    set_name("Tiamat");
    set_id("dragon", "tiamat");
    set_gender(1);
    set_proper_name("Tiamat");
    set_in_room_desc("Tiamat");
    set_long("Getting close enough to see what a dragon is not a good idea.");

    set_max_health(300);
    set_wander_time(5);
    set_wander_area("wiz_area");
}
