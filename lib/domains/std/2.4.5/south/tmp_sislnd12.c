/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(1);
set_brief("The shore of the Isle of the Magi");
set_long("You are standing on the shore of the Isle of the Magi\n The shore of the island continues northeast to the ruined bridge\n and west into a small grove from here.\n To the south is a old, disused well.\n Standing atop a cliff to the southwest is the ruined tower of Arcanarton,\n but there is no way to get up there from here.\n A path does lead up the hill to the east though.");
set_exits( ([
  "south" : "sislnd17.scr",
  "east" : "sislnd13.scr",
  "west" : "sislnd11.scr",
  "northeast" : "sislnd1.scr",
]) );
}

