/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit ROOM;

create(){
    room::create();
    set_brief("The Toy Room");
    set_long("There isn't anything here yet cos Ohara hasn't got round to it.\n");
    set_light(1);
}

