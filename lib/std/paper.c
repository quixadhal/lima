/* Do not remove the headers from this file! see /USAGE for more info. */

// WIP

inherit OBJ;
inherit M_LIGHTABLE;
inherit M_GETTABLE;
inherit M_READABLE;

int burnt;
float max_fuel;


void mudlib_setup()
{
    set_fuel( 4 );
    set_id( "paper" );
    set_size( VERY_SMALL );
    set_light_level( 0 );
    set_die_msg( "The $o crumbles into fine ash." );
}

void mangle_text()
{
    string my_text = query_text();
    int text_size = sizeof( my_text );
    int i;

    if( sizeof( my_text ) > 0 )
    {
	if( !burnt )
	{
	    burnt = 1;
	    my_text = "It's somewhat burnt, but reads as follows:\n" + my_text;
	}
	for( i = 0; i < text_size; i++ )
	    if( random( max_fuel ) > current_fuel())
		if( !(my_text[i] == ' ' && random(1))) my_text[i] = '?';
	set_text( my_text );
    }
}

mixed extinguish()
{
    mangle_text();
    return ::extinguish();
}

string read()
{
    if( query_is_lit())
	mangle_text();
    return ::read();
}

void set_fuel( int amount )
{
    if( !burnt ) max_fuel = amount;
    ::set_fuel( amount );
}
