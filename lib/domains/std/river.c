/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_FLUID_SOURCE;
inherit M_DRINK_SOURCE;

void setup() 
{
    set_adj ( "rushing" );
    set_id("river", "water" );
    set_long( "The river rushes madly by."
	 	);
    set_size( TOO_LARGE );
    set_attached( 1 );
    set_source_of ( WATER );
    set_fluid_disturb_action( "You get very wet." );
    set_fill_action( "$N $vplunge into the river to get some water into $p $o.");
    set_drink_action( "$N $vplunge $p hands into the river, attempting to get some water.");
}

