/* Do not remove the headers from this file! see /USAGE for more info. */

inherit SIGN;

void setup()
{
    set_in_room_desc("A small sign floats comfortably in mid-air.");

    set_long("Perhaps you should read it"); 
    set_text( @ENDMARKER
Welcome to the wonderful world of Lima Bean, created by Rust,
Deathblade and Beek.  This rooms and the objects here are designed to
be examples of how to effectively use the Lima mudlib.  Please feel
free to make comments and suggestions.
ENDMARKER );   

}
