/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_FLUID_SOURCE;
inherit M_DRINK_SOURCE;

void setup() 
{
    set_id("keg");
    set_long( "A great round keg of ale.");
    set_size( TOO_LARGE );
    set_source_of ( __DIR__ "ale" );
    set_in_room_desc( "A keg of ale stands against one wall." );
    set_long ( "It looks like a fat barrel laying on its side. "
		"On the side of the keg is a logo for the Frobozz Magic Ale Corporation."
		 );
    set_fluid_disturb_action( "A head of foam forms on the ale." );
    set_fill_action( "$N $vturn on the tap of the keg and $vpour some ale into $p $o." );
    set_drink_action( 0 );
}

