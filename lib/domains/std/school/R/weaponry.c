#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Wizard's Weaponry");
    set_long(@KAAN
  The blacksmith hall where weapons are created. There is a large anvil 
and a flat screen computer floating in the air suspended by a 
mechanical armature. Along the shiny white walls are several racks.
KAAN
    );

    add_item("anvil",  
      ([ "look" :(@KAAN
Its a white hot anvil made the hardest substance you have ever seen. This 
preheated source for smithing eliminates the need for a hearth to forge 
weapons. A smith only has to place his item on it and state it to be 
melted to the percentage of the object's melting point.
KAAN),             
      ]) );


    add_item("computer",  
      ([ "look" :(@KAAN
Yep its a neat computer. Its marvelously thin at only 1 inch thick 
without a cluncky box to go with it because its all imbeded inside the 
back of the monitor like a PADD.
KAAN),             
      ]) );

    add_item("walls",  
      ([ "look" :(@KAAN
The walls are bright and clean. They they seem to be made of a substance 
that is heat sinking to keep the shop cool. There are two racks of 
current weapon types. One consisting of lightsabers and another 
consisting of swords. Under the racks is a sign that reads "Under 
Construction. More weapon types to come soon to a blacksmith near you."
KAAN),             
      ]) );

    set_exits( ([
	"corridor" : R "corridor06",
	"basic" : R "sword01",
      ]) );
    set_objects( ([
      ]) );
}
