#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 6");
    set_long(@MAY
6) Flow of sentences: I myself have problems with this, sometimes. The flow of
sentences should go smoothly, and not be chopped up. If your description starts
wit the sentence like in #4, and is followed by "A large bowl sits in the
middle of the floor. The walls are lined with burned out torches." It's kinda
like saying" This is here. That is there. Oh, and this has this other thing."
It tends to confuse players. Again, remember how your favorite Author writes
descriptions and apply that.


Remember, there is ALWAYS at least one thing to describe in a room, usually
the floor, even if it is just as simple as saying it looks like it needs to 
be cleaned.
MAY
    );
    set_exits( ([
	"area5" : R + "app_part5",
	"area7" : R + "app_part7",
      ]) );
    set_objects( ([
      ]) );
}
