/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;

void setup()
{
  set_id("bottle", "flask");
 set_long("Appears to be a normal looking bottle. ");
set_gettable(1);
}

void drink_it()
{
  this_body()->simple_action("$N $vtake a drink from the bottle.\n");
}
int direct_drink_from_obj()
{
 return 1;
}
int direct_drink_obj()
{
   return 1;
}
