#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 6");
  set_long(@MILAMBER
stupidemote
----------------------------------------------------------------------
This is one of the two commands which will automatically create basic
emotes.  It creates an emote and adds two default rules.
  Syntax:
    stupidemote <emote>
    ie: "stupidemote smile"
The above example would create a smile emote with the following rules:
  ""    -> $N $vsmile.    // Default
  "STR" -> $N $vsmile $o. // String argument
MILAMBER
  );
  set_exits( ([
    "north" : R + "emote07",
    "south" : R + "emote05",
   ]) );
  set_objects( ([
   ]) );
}
