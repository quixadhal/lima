#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 4");
    set_long(@MAY
4) Picture yourself walking into a room: Picture yourself walking into the room
you're describing. First, describe the room itsself at a quick glance "A small
room, cluttered with various things scattered along the floor. The two windows
in the room have dark colored curtains, blocking most of the sunlight from
entering. Posters line the walls and a bed sits in the southwest corner of the
room.

Then, you make the items in the room. First things, thing and floor "The floor
is cluttered with all sorts of electronic equipment on different styles of
stands." Next, window, windows, curtain, and curtains "Upon closer inspection,
the curtains are mostly dark blue and have an Empire Strikes Back motif." Next,
posters, poster, walls and wall "The walls are papered with posters of female
super models." Next, the bed "It's just an ordinary twin sized bed covered with
a Spider-man quilt. An unusually large lamp sits atop the headboard."
MAY
    );
    set_exits( ([
	"area3" : R + "app_part3",
	"area5" : R + "app_part5",
      ]) );
    set_objects( ([
      ]) );
}
