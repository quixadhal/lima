/* Do not remove the headers from this file! see /USAGE for more info. */

inherit STOCK_MASTER;

void create()
{
    ::create( "stock-fighter" );
}

void receive_outside_msg( string str )
{
    ::receive_outside_msg( str );
}
