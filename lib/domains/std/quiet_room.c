/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("Quiet Room");
    set_long("This small and neat place is the Quiet Room. Originally intended to be a place where wizards can come to read the newspapers and do other tasks without all the distractions of the Grand Hall, due to the invincible stubbornness of human nature, it has become the spot for many a holy war. Still, if a person sits here in the early hours of the morning, it is still relatively quiet and peaceful.");
    set_light(1);
    set_exits( ([
 "east" : "/domains/std/wizroom.c"
    ]) );
}


