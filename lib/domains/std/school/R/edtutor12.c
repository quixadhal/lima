// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 12");
  set_long(@MAY
ed testroom.c
n
1z
z
x

ed testroom.c
n
1,$p
x


ed testroom.c
n
1,$p
w
q

Emergency way out 
ed testroom.c
n
Q
MAY
  );
  set_exits( ([
    "south" : R + "edtutor11",
    "corridor" : R + "corridor01",
   ]) );
  set_objects( ([
   ]) );
}
