/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit ROOM;

void setup()
{
set_area("wiz_area");
    set_brief("General Store");
    set_long(
	"This is the general store.  Players come here to sell items collected\n"
	"during their adventures, and to buy items needed for adventuring.\n"
	"There is a sign on the west wall please read it.\n"
	);

    add_item("sign", ([
"look" : "Please ask biff about (swords, keys, etc),\n"
         "       buy (sword or sword #) from biff,\n"          
         "       sell (sword, first sword etc) to biff.\n",
"read" : "Please ask biff about (swords, keys, etc),\n"
         "       buy (sword or sword #) from biff,\n"          
         "       sell (sword, first sword etc) to biff.\n",
	]) );
    set_objects( (["/domains/std/shopkeeper.c" :1]) );

    set_exits( ([ "up" : "wizroom" ]) );
}
