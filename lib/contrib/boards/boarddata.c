/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** boarddata.c
**
** Provides the player data for the boards.
**
** Quick hack from body/newsdata.c by Fritti.
*/

#include <daemons.h>

private mapping board_data = ([]);

nomask int query_board_id(string board)
{
    return board_data[board];
}

nomask void set_board_id(string board, int id)
{
    board_data[board] = id;
}
