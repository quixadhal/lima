/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_light(0);
set_brief("The bottom of the well");
set_long("You are standing at the bottom of the well, about thirty feet below the\n surface. Bones lie strwen about in a random fashion, many of them broken\n or shattered.\n \tThe only way out is the way in, back up the ladder.");
set_exits( ([
  "up" : "sislnd17.scr",
]) );
set_objects( ([
  "/domains/std/2.4.5/south/tmp_lair_0.scr" : 1,
]) );
}

