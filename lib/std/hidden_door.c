/* Do not remove the headers from this file! see /USAGE for more info. */
// By Ohara
// Revised by Marroc, February 4, 2001.

inherit DOOR;

void do_hidden_stuff()
{
    if( query_closed() )
      set_hidden(1);
    else
      set_hidden(0);
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
    set_attached(1);

}

