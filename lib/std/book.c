/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_OPENABLE;
inherit M_READABLE;

int
read()
{
  if( !is_open() )
    return (int)notify_fail( "Perhaps you should open the book first.\n" );

  return readable::read();
}

int
read_entry( string entry )
{
  if( !is_open() )
    return (int)notify_fail( "Perhaps you should open the book first.\n" );

  return readable::read_entry( entry );
}
