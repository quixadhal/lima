#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 7");
    set_long(@MAY
7) Diversity & Sentence Structure : Try and make each sentence begin with a
different word. If you diversify the room and use different words that mean the
same thing, it makes the sentence clearer. Avoid run-on sentences and choppy
sentences. Example: "The wall is painted black. A black chair sits in the
northeast corner. A black end table sits next to the chair." might read better
"The walls are painted a deep, dark ebony. In the northeast corner sits a shiny
black chair. An end table made from what appears to be solid black onyx sits
just to the right of the chair."

I know this isn't English class, and you all aren't aspiring writers. But, if
you read and follow these guidelines, it will not only improve your writing,
but your vocabulary and possibly other aspects of your life as well. It will
also make the mud a better environment for the players. And never be afraid to
ask for help to fix your descriptions if they need to be worked on.
MAY
    );
    set_exits( ([
	"area6" : R + "app_part6",
 	"corridor" : R + "corridor03",
     ]) );
    set_objects( ([
      ]) );
}
