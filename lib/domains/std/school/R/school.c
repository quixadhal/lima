// May - 2000

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Wizard's Academy");
  set_long(@MAY
   This is an abnormally large propylaeum to the Wizards Academy here
on Lima Bean. A large gate towers so high here that one has to crack 
their neck to see the top of it. On the left and right side of the gate
are two massive Gamorrean guards, who grunt and snort while waiting 
for their next trespasser to prey upon. On the other side of the gate 
is a long stoa lined with tombs that contain sacred knowledge of the 
ancients only those who have become masters or gained tutelage under 
them can comprehend these texts. Impressive this vantage point may be, 
there are many things to learn.
MAY
  );

  add_item("gate",  
   ([ "look" :(@KAAN
   The gate is approximately 30 to 40 feet high. It is made of Laboi 
Crystals. The form work of the gate is beyond measure. To begin to 
"up" : R + "newwiz1",
describe it would fail to do it justice. Instead it can only be 
passionately explored to its indescrible detail. One aspect really 
paradoxically shines is a titanium doorbutton, which blips and flickers.
KAAN),             
   ]) );

  add_item("crystals", 
   ([ "look" :(@KAAN
   These are the crystals of Laboi. They are highly sought through out 
the universe for its rarity and beauty. However, Laboi crystals can only 
be cultivated by a gentle mammalian race that live on Laboi II. The Laboi 
are a nomadic tribe who search for the dangerous yans. One important 
aspect of developing these crystals is they need telekinesis to affect 
how they grow and shape. Ponder the one who was powerful enough to form 
these crystals into a monolithic gate.
KAAN),             
   ]) );

  add_item("doorbutton",
   ([ "look" :(@KAAN
   The titatium door button has different hues of blue and silver. It 
blips and bloops to a presence that approaches it. The lights glow into 
red and white making most familar sounds. Aside the light is a port. Its 
complex circuitry requires some knowledge.
KAAN),             
   ]) );

  add_item("port",
   ([ "look" :(@KAAN
   The port is circular with varying lenths of radial angles. 
Interlocking plates whir momentarily and make fasciating geometric 
combinations. The answer to this intresting port lies only in the memory 
of the past. Also in the port is a hollowed out core.
KAAN),             
   ]) );


  add_item("light",
   ([ "look" :(@KAAN
    Yep its a light, nothing fascinating about the light. Except, maybe 
the fact it changes colors from white to blue and red most fluidly. 
Apparently the light is filled with a vitreous fluid is remarkably cool 
to the touch. It is probably made of a crystalline type of fluid.
KAAN),             
   ]) );


  add_item("guard",
   ([ "look" :(@KAAN
Which guard? There are two guards. left or right?
KAAN),             
   ]) );

  add_item("left guard",
   ([ "look" :(@KAAN
  The short Gamorrean guard looks like a joke. However, it would not be 
wise to under estimate him. For he is not just short but very stout and 
heavy boned. He wields a 40 centimenter vibro blade. He is adorned with 
bare essentials and reeks of the most foul odors. His cousin guards the 
right side some thing is particular about the right guard.
KAAN),             
   ]) );

  add_item("right guard",
   ([ "look" :(@KAAN
This is an abnormally large guard, especially, for a Gamorrean guard. He 
stands 2.5 meters tall. Yet, in addition to to being tall he is massively 
built. One would estimate he is about 500 kilograms. Certainly such a 
rarity in Gamorrean guard must come high priced. This leads to some 
assumptions about the size of the left guard.
KAAN),             
   ]) );

  add_item("stoa",
   ([ "look" :(@KAAN
Alas, my friend, you can see good but you don't have x-ray vision through 
the gate. Also, its telekinetic nature hinders force-vision as well.
KAAN),             
   ]) );

  add_item("tomes",
   ([ "look" :(@KAAN
Alas, my friend, you can see good but you don't have x-ray vision through 
the gate. Also, its telekinetic nature hinders force-vision as well.
KAAN),             
   ]) );

  set_exits( ([
    "newwiz" : R "newwiz1",
    "forward" : R "corridor01",
   ]) );
  set_objects( ([
    O "map" : 1,
   ]) );
}
