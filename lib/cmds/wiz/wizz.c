/* Do not remove the headers from this file! see /USAGE for more info. */

// product du Belboz/Rust

//:COMMAND
//$$ see: goto, trans
//USAGE wizz
//
//This command returns you to the "wiz room" from anywhere on the mud.

#include <move.h>

inherit CMD;

string* MSGS;

create()
{
    MSGS =	({
    "a statue of Dimwit Flathead",
    "a Frobozz magical broom",
    "supermodel Elle McFlathead",
    "a fearesome hellhound",
    "a crowd of Elvis Flathead fans",
    "the boot patrol",
    "a giant brogmoid",
    "the Wings of Icarus",
    "the mighty dragon Thermofax",
    "laughing eldritch vapors",
  });
}

private void main( string arg )
{
  object	start_room;
  string	name;

  start_room = environment( this_body() );
  name	= this_body()->query_name();

  if( WIZ_ROOM == file_name(start_room))
  {
    this_body()->simple_action("$N $vtwitch briefly.");
    return;
  }
  if(!arg)
    arg = choice(MSGS);

  if( this_body()->move( WIZ_ROOM ) != MOVE_OK)
  {
    out( "You remain where you are.\n" );
    return;
  }
  this_body()->force_look();
  outf("You are carried to the wizard lounge on %s!\n", arg );
  if( name == "Someone" )
    return;

  tell_from_inside( start_room, sprintf( "%s is whisked away on %s!\n",
      name, arg ) );
  tell_environment( this_body(),
    sprintf( "%s arrives on %s!\n", name, arg ), 0, ({this_body()}) );
}

