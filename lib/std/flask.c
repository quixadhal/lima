/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_GETTABLE;
inherit M_FLUID_CONTAINER;

void setup()
{
    set_adj("clear", "glass", "blown");
    set_id("flask", "bottle");
    set_long( "It is a thick flask blown from clear glass." );
#ifdef USE_SIZE
    set_size(  1  );
#endif
#ifdef USE_MASS
    set_mass(  1  );
#endif
    add_relation("in",1);
    set_default_relation("in");
}
