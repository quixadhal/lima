#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 7");
  set_long(@MILAMBER
targetemote
----------------------------------------------------------------------
This is the other autoemote command.  It creates an emote with four
basic rules.
  Syntax:
    targetemote <emote>
    ie: "targetemote smile"
This would create a smile emote with the following rules:
  "LIV" -> $N $vsmile at $t.  // Living arg.
  "OBJ" -> $N $vsmile at $o.  // Object arg.
  "STR" -> $N $vsmile $o.     // String arg.
  ""    -> $N $vsmile.        // Default
MILAMBER
  );
  set_exits( ([
    "corridor" : R + "corridor06",
    "south" : R + "emote06",
   ]) );
  set_objects( ([
   ]) );
}
