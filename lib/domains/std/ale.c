/* Do not remove the headers from this file! see /USAGE for more info. */

inherit FLUID;
inherit M_DRINKABLE;


void setup() 
{
    add_adj( "amber" );
    set_id( "ale", "beer", "lager");
    set_size( 1 );
    set_long( "The ale is amber and clear, with a slight head of foam on it. ");
    set_in_room_desc("There is a puddle of spilled ale on the ground here.");
    set_gettable( "#It slips through your fingers."  );
    set_evaporate_action( "The puddle of ale dwindles." );
    set_taste_action( ({"It's a little bit weak, but it'll do the job.",""}) );
    set_quantity( 1 );
}

