/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CHEST;

void setup() 
{
  set_adj("large", "magic");
  set_long( "It's a large chest, that glows softly with a magical aura.");
  set_objects( ([
		 "chainmail" : -1,
		 "objects/sword" : -1,
		 ]) );
  set_gettable( "#It's way too heavy to lift.\n" );
  set_locked("chest");
  set_strength_vs_magic(0);
  set_strength_vs_picking(100);
}
