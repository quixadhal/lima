/* Do not remove the headers from this file! see /USAGE for more info. */
// By Ohara

inherit DOOR;

string our_dir;

void do_hidden_stuff()
{
    object here;
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


varargs void on_clone( string dir, string room,mixed rest... )
{
    ::on_clone( dir, room, rest...);
    do_hidden_stuff();
}


void setup_door( string ident, string dir, string room ) 
{
    ::setup_door( ident, dir, room );
    our_dir = dir;
    set_flag( ATTACHED );
}

