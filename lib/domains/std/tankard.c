/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_GETTABLE;
inherit M_FLUID_CONTAINER;

void setup()
{
  set_adj("shiny", "metal");
  set_id("tankard");
  set_long( "It is a tankard made from shiny metal." );
#ifdef USE_SIZE
  set_size( 4 );
#endif
#ifdef USE_MASS
  set_mass( 4 );
#endif
  add_relation("in", 8);
  set_default_relation("in");
}
