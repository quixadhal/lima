/* Do not remove the headers from this file! see /USAGE for more info. */
// By Ohara


inherit DOOR;


void update_state( object obj )
{
    object there = environment( this_object());
    ::update_state( obj );
    if( obj->query_closed())
        there->add_hidden_exit( our_dir );
    else
        there->remove_hidden_exit( our_dir );
}


void set_closed( int x )
{
    object here = environment(this_object());

    ::set_closed( x );
    if( !here )
        return;
    if( x )
        here->add_hidden_exit( our_dir );
    else
        here->remove_hidden_exit( our_dir );
}


void setup_door( string ident, string dir ) 
{
    ::setup_door( ident, dir );
    set_flag( ATTACHED );
}

