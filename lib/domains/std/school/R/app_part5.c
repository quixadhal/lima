#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 5");
    set_long(@MAY
Then, you go into the item descriptions for the items described in the other
item descriptions. It's sort of like layering the descriptions based on how you
would really look at them when you enter a room. At first glance, you see the
basics. Next, you look at groups of objects such as posters and windows. Then,
you look at specifics such as TV and lamp. A good idea is to make a room that
is EXACTLY like your real life bedroom(like I just did), and use it for guide
for future rooms. Don't forget, just because I didn't say there was a ceiling,
doesn't mean it isn't there. And, it may have something unusual on it, such as
a confederate flag or christmas lights that you wouldn't necessarily see just
walking into the room unless you looked specifically at the ceiling. 

5) Avoid the use of "you" and "your" in room descriptions: Not only does this
make for a bad description, especially in rooms, it makes the MUD god angry.
Start a description with something like "A long, dark hallway stretches east
and west." Item descriptions are ok to use you and your.
MAY
    );
    set_exits( ([
	"area4" : R + "app_part4",
	"area6" : R + "app_part6",
      ]) );
    set_objects( ([
      ]) );
}
