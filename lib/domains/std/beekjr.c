/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit LIVING;

void create() {
    ::create();
    set_id( ({ "beekjr" }) );
    set_in_room_desc("Beek Jr is here, standing off to one side of the room.");
}
