// Beek - July 30, 1995
//
// Interface to the parse_sentence() debug info

#include <mudlib.h>

inherit CMD;

private void main(string str) {
    this_body()->do_game_command(str, 1);
}
