#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 5");
  set_long(@MILAMBER
Additional Notes:
additional emote commands:
rmemote
----------------------------------------------------------------------
This command is used to remove emotes from the soul or to remove one
rule of an emote from the soul.
  Syntax:
    rmemote <emote>
    ie: "rmemote smile"
      This removes the entire emote and all of its rules.
    rmemote <emote> <rule>
    ie: "rmemote smile LIV"
      This removes only the rule and the message for that rule. It
      leaves all other rules alone.

MILAMBER
  );
  set_exits( ([
    "north" : R + "emote06",
    "south" : R + "emote04",
   ]) );
  set_objects( ([
   ]) );
}
