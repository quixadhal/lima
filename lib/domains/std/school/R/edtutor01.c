// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 1");
  set_long(@MAY
Welcome to the world of ED.  In this land you will learn about the
editor that the Wizards use here at Beyond the Flame.  It is called ED.
On your journey through these tutorial rooms, pay careful attention
to the things that Ed has to say.  He is an expert when it comes to
this editor.  He should be, it was named after him.
   Now once you are in ED, you have a new bunch of commands that
you must learn.  Remember, all the other commands that you
normally use as a wizard like <goto> and <tell> will not work
WITHIN ED.  So, what you must do if you want to use a 'regular'
character command is put a <!> before the statement.  This
tells ED that the command is not an ED command but is a regular
command and allows you to execute that command.  This will
be very helpful when you have questions to ask others when
you are editing.
   So to begin on your journey into the land of ED, just type
<north>.  The same command holds true throughout these
rooms.  To go south to the previous room, type <south>.  That is all
you need to know to get you started using the program ED.
MAY
  );
  set_exits( ([
    "north" : R + "edtutor02",
    "corridor" : R + "corridor01",
   ]) );
  set_objects( ([
   ]) );
}
