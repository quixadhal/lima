/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
/* no castle drop here... its a jetty, how can anything be placed north &
   south of here... there is nothing but water around, place it in sea */
#undef EXTRA_RESET
#define EXTRA_RESET\
    no_castle_flag=1;\
    if (!present("bag"))\
        move_object(clone_object("obj/bag"), this_object());

void setup() {
function f;
set_exits( ([
  "west" : "vill_shore2.scr",
  "east" : "sea.scr",
]) );
set_brief("Jetty");
set_long("You are at a jetty. The waves rolls in from east.\nA small path leads back to west.");
set_light(1);
}

