/* Do not remove the headers from this file! see /USAGE for more info. */

// Prototypical crowd
//
// Idea by Ohara
// Code by Beek
//
// This can move to a /std/crowd after proof-of-concept

#include <mudlib.h>

inherit LIVING;

string long_func() {
    if (!present("beekjr", environment()))
	new("/domains/std/beekjr")->move(environment());
    return "As you look at the crowd, you notice Beekjr standing off to one side.";
}

void setup() {
    set_id("crowd");
    set_in_room_desc("A crowd of LPC coders mills around.");
    set_long( (: long_func :) );
}
