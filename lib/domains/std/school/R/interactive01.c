#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Interactive npc");
    set_long(@MAY

Adapt from the example used in the basic or simple npc.

Start by adding periodic actions, in set_actions.
First argument - pause between actions (seconds)
Second argument - string for action, or string array for alternatives.

  set_actions( 10, ({
    "say What are you waiting for?",
	  "say Hello there!",
	  "say I don't like winter.",
	  "say I don't like snow.",
  }) );

MAY );

    set_area("wiz_area");

    set_exits( ([
	"backward" : R "zoo",
	"forward" : R "interactive02",
      ]) );

   set_objects( ([
     "/domains/std/harry" : 1,
     ]) );
}
