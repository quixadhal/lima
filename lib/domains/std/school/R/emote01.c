#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 1");
  set_long(@MILAMBER
command   : addemote
help file : /help/wizard/bin/addemote

Be sure to read the real help files along with this little tutorial to
be sure that I don't forget anything.  This command comes in three (3)
steps.  The first is to enter the name of the emote.  This is done as
follows: "addemote <verb>" (ie: "addemote smile").

The next step is to enter the rule.  Let me give some examples to
explain.  If I was to type "smile lancelot", it would call the emote
"smile" and since lancelot is a living, it would apply the rule "LIV".
If I was to type "smile broadly lancelot", it would call the rule
"STR LIV" b/c broadly is a string and lancelot is alive.  The main
rules are: LIV = living in room (Players/Monsters), OBJ = Object in
room, and STR = anything else.  You can also mix text in with the rule.
Like if I typed: "smile broadly at lancelot" it would apply the rule:
"STR at LIV", get it?

MILAMBER
  );
  set_exits( ([
    "north" : R + "emote02",
    "corridor" : R + "corridor06",
   ]) );
  set_objects( ([
   ]) );
}
