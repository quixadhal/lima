/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("Monster Room");
    set_long("This is the Monster Room of LIMA. This is the place where wizards summon their pets into - to be introduced to other wizards. There aren't any around yet, since everybody is waiting for the combat sequences to be completed. The Grand Hall is to the north.");
    set_light(1);
    set_exits( ([
 "north" : "/domains/std/wizroom.c"
    ]) );
}


