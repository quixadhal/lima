/* Do not remove the headers from this file! see /USAGE for more info. */
// By Ohara


inherit DOOR;


void do_hidden_stuff()
{
    object here;
    string our_dir = "northeast";
    here = environment(this_object());

    if (!here) return;
    if( query_closed() )
	here->add_hidden_exit( our_dir );
    else
	here->remove_hidden_exit( our_dir );
}


void set_closed( int x )
{
    object sib = get_sibling();

    ::set_closed( x );
    do_hidden_stuff();
    if( sib ) sib->do_hidden_stuff();
}


varargs void on_clone( string ident, string dir, string room )
{
    ::on_clone( ident, dir, room );
    do_hidden_stuff();
}


void setup_door( string ident, string dir, string room ) 
{
    ::setup_door( ident, dir, room );
    set_flag( ATTACHED );
}

