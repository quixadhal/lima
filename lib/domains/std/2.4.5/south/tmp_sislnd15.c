/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/indoor_room";

void setup();
look(str)
{
    if (str != "at monument")
	return 0;
    else
	{
	    write("The monument is very old and is crumbling.\n" +
		  "Affixed to the side of the monument is a corroded old plaque which reads:\n" +
"+-------------------------------------------------------------------------+\n"+
"|                   BEWARE ALL YE WHO READ THIS MESSAGE                   |\n"+
"|        Be it known, that on this day, the tower of the evil mage        |\n"+
"|       Arcanarton was destroyed in an attack by the combined forces      |\n"+
"|               of all of the mages of the land of Lustria.               |\n"+
"|     The body of the mage Arcanarton was not found, and it is feared     |\n"+
"|             that his evil work in this world is not yet over.           |\n"+
"+-------------------------------------------------------------------------+\n");
	}
}
void setup() {
function f;
set_light(1);
set_brief("A crumbling stone monument");
set_long("You are halfway up the hill.\n A crumbling monument stands on the side of the hill here.\n On top of the hill, to the west, stands the ruins of the tower of\n Arcanarton.\n A path winds around the hill to the north, and heads down to the shore\n of the island to the south and east\n");
set_exits( ([
  "north" : "sislnd14.scr",
  "south" : "sislnd6.scr",
  "east" : "sislnd5.scr",
  "west" : "sislnd18.scr",
]) );
}

