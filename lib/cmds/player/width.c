/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//USAGE width
//      width <number>
//
//Allows you to see (or set) what the mud will use as your screen width when
//formatting information.
//It's probably a good idea to set it to the same as your client' setting,
//so they don't get confused when talking to each other.

inherit CMD;

void main( mixed width )
{
  width = to_int(width);
  if( !intp( width ))
  {
    out( "Must set screen width to an integer.\n");
    return;
  }
  if( !width )
  {
    out( "Your current screen width is " + this_user()->query_screen_width() + ".\n" );
    return;
  }
  if( width < 10 )
  {
    out( "Screen width must be 10 or greater.\n");
    return;
  }
  this_user()->set_screen_width( width );
  out( "Screen width set.\n");
}
