#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Wizard's Armory");
    set_long(@MAY
The armory.
MAY
    );
    set_exits( ([
	"corridor" : R "corridor06",
	"amulet" : R "amulet",
	"gloves" : R "gloves",
	"shield" : R "shield",
	"boots" : R "boots",
	"platemail" : R "platemail",
	"basic" : R "chainmail01",
      ]) );
    set_objects( ([
      ]) );
}
