/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "west" : "vill_road2.scr",
  "east" : "jetty.scr",
  "north" : "eastroad1.scr",
  "south" : "crop.scr",
]) );
set_brief("Road");
set_long("You are on a road going out of the village. Eastroad runs north from here,\n along the eastern perimeter of the city, and to the south are some fields\n planted with all the crops that the city needs. The main road runs towards\n the shore to the east, and into the city to the west.");
set_light(1);
}

