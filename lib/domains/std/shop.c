/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

void create()
{
   ::create();
     set_brief("General Store");
set_long(
"This is the general store.  Players come here to sell items collected\n"
"during their adventures, and to buy items needed for adventuring.\n"
"There is a sign on the west wall please read it.\n"
);

add_item("sign", ([
                                 "look" :"THIS SHOP IS STILL UJNDER CONSTRUCTION",
"read" :"THIS SHOP IS STILL UNDER CONSTRUCTION"
]) );
set_objects( (["/domains/std/shopkeeper.c" :1]) );

set_exits( ([ "up" : "wizroom" ]) );
}
