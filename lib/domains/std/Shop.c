/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;




void setup()
{
    set_area("wiz_area");
    set_brief("General Store");
    set_long(
      "This is the general store.  Players come here to sell items collected during their adventures, and to buy items needed for adventuring. There is a sign on the west wall please read it."
    );

    add_item("sign", ([
	"look" : "Please ask biff about (swords, keys, etc),\n"
	"       buy (sword or sword #) from biff,\n"          
	"       sell (sword, first sword etc) to biff.\n",
	"read" : "Please ask biff about (swords, keys, etc),\n"
	"       buy (sword or sword #) from biff,\n"          
	"       sell (sword, first sword etc) to biff.\n",
      ]) );
    set_objects( ([
	"/domains/std/shopkeeper.c" : 1,
	STAIRS : ({ "/domains/std/Wizroom", 0 }),
      ]) );

}
