/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit INDOOR_ROOM;

void setup(){
    set_area("wiz_area");
    set_brief("Monster Room");
    set_long("This is the Monster Room of LIMA. This is the place where wizards summon their pets into - to be introduced to other wizards. There aren't any around yet, since everybody is waiting for the combat sequences to be completed. The Grand Hall is to the north.");
    set_exits( ([
	"north" : "wizroom.c"
      ]) );
    set_objects(([ "/domains/std/guild_guard" : ({ "sorcery" }) ]));
}
