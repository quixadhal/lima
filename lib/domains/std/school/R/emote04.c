#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Emote Tutorial 4");
  set_long(@MILAMBER
Additional Notes:
  * addemote can also be used on existing emotes to modify them or to
    add more rules.
  * Each rule must be created individually, there are a shortcuts, but
    we'll deal with them in one of the future lessons.
  * To modify an existing emote rule, just pretend you are making a new
    one.  Type "addemote <verb>" then enter the rule you want to change,
    then enter the new message.  Nothing to it.

Very Important Additional Note:
  * To see all the rules and messages for an existing emote use the
    command "showemote".  The syntax is "showemote <verb>"  This is a
    very useful tool for learning how to code emotes b/c you can easily
    see all the rules in action.
MILAMBER
  );
  set_exits( ([
    "north" : R + "emote05",
    "south" : R + "emote03",
   ]) );
  set_objects( ([
   ]) );
}
