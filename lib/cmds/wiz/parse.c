/* Do not remove the headers from this file! see /USAGE for more info. */

// Beek - July 30, 1995
//
// Interface to the parse_sentence() debug info

#include <mudlib.h>

inherit CMD;

private void main(string str)
{
    int flag = 1;

    if ( sizeof(str) > 3 && str[0..2] == "-v ")
    {
        flag = 2;
        str = str[3..];
    }

    this_body()->do_game_command(str, flag);
}
