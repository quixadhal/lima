/* Do not remove the headers from this file! see /USAGE for more info. */

/* A basic piece of furniture.  This one happens to be a bed (go figure).
 * It also has an example of a use of add_method, as this really is a complex
 * exit. */
inherit FURNITURE;

void mudlib_setup()
{
  ::mudlib_setup();
  set_id("bed");
  set_in_room_desc("There is a bed in the corner of the room.");
  set_long("It's a bed...");
  set_default_relation("on");
  add_relation("under",VERY_LARGE);
  set_max_capacity(VERY_LARGE*3, "on");
  /* NOTE: 'sit on' is already added by FURNITURE */
  add_method("lie on",this_object(),(:enter_check:),"$N $vlie on the bed");
  /* This probably isn't a very good capacity for under */
  add_method("crawl under",this_object(),(:enter_check:), 
	     "$N $vcrawl under the bed");
}

