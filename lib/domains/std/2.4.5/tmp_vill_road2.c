/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_objects( ([
  "/domains/lpscript/harry.scr" : 1,
]) );
set_exits( ([
  "west" : "vill_road1.scr",
  "east" : "vill_shore.scr",
  "south" : "adv_guild.scr",
  "down" : "station.scr",
  "north" : "shop.scr",
]) );
set_brief("Village road");
set_long("A long road going through the village. There are stairs going down.\n The road continues to the west. To the north is the shop, and to the\n south is the adventurers guild. The road runs towards the shore to\n the east.");
set_light(1);
}

