#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Interactive npc - responses");
    set_long(@MAY

This is only the simple part of an extensive subject.

Adapt from the simple npc and inherit M_TRIGGERS.

Within setup(), add the response patterns.
Each consists of 2 strings - trigger to match, and what to do in response.

  add_pattern("%s laughs", "say Did I miss a joke?");
  add_pattern("%s chuckles", "smirk");
  add_pattern("%s enters", (: "say Hi " + $1 + ", nice to see you !" :) );
Note the last case picks up who enters, in the %s,
then uses it in $1 in the response.

MAY );

    set_exits( ([
	"finish" : R "zoo",
	"backward" : R "interactive01",
      ]) );

   set_objects( ([
     "/domains/std/harry" : 1,
     ]) );
}
