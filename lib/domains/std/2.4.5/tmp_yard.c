/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();

void setup() {
function f;
set_exits( ([
  "south" : "vill_road1.scr",
  "east" : "pub2.scr",
]) );
set_brief("Small yard");
set_long("A small yard surrounded by houses.");
set_light(1);
set_objects( ([
  "/domains/std/2.4.5/tmp_yard_0.scr" : 1,
  "/domains/std/2.4.5/tmp_yard_1.scr" : 1,
]) );
f = function(function f) { {
switch (random(3)) {
case 0: {
{
this_body()->do_game_command("say Please, give money to a poor beggar!");
;
}
break; }
case 1: {
{
this_body()->do_game_command("say Why can't I find any money ?");
;
}
break; }
case 2: {
{
this_body()->do_game_command("say Beggar says: two coins please !");
;
}
break; }
}
;
}
 call_out(f, 1 + random(98), f); }; call_out(f, 1 + random(98), f);
}

