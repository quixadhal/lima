/* Do not remove the headers from this file! see /USAGE for more info. */

inherit MONSTER;
inherit M_BLOCKEXITS;

void setup() {
    object sword;

    set_name("Tiamat");
    set_id("troll", "tiamat");
    set_gender(1);
    set_proper_name("Tiamat");
    set_in_room_desc("Tiamat");
    set_long("Getting close enough to see what a dragon is not a good idea.");

    set_max_hp(300);

}

mixed take_a_swing(object target)
{
    if(random(20)) return ::take_a_swing(target);
    foreach(target in query_target())
    {
	if(target->is_living()) continue;
	{
	    write(" just testing foo you got hit");
   target->do_damage(50, "FIRE");
   }
   }
}
