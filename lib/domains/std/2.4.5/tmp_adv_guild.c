/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
mixed can_go_south() {
    if (wizardp(this_user()))
        return 1;
    this_body()->other_action("$N $vtry to go through the field, but fails.\n");
    return "A strong magic force stops you.";
}

mixed do_go_south() {
    write("You wriggle through the force field...\n");
    return 0;
}
void setup() {
function f;
set_light(1);
set_exits( ([
  "north" : "vill_road2.scr",
  "south" : "adv_inner.scr",
]) );
set_brief("The adventurers guild");
set_long("You have to come here when you want to advance your level.\n You can also buy points for a new level.\n There is a list of quests on the wall.\n There is an opening to the south, and some shimmering\n blue light in the doorway.");
set_objects( ([
  "/domains/std/2.4.5/tmp_adv_guild_0.scr" : 1,
  "/domains/std/2.4.5/tmp_adv_guild_1.scr" : 1,
  "/domains/std/2.4.5/tmp_adv_guild_2.scr" : 1,
]) );
}

