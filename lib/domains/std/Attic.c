/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;


mixed can_go_down(){ return 1; }
int do_go_down()
{
    this_body()->force_game_command( "climb down stairs");
    return 1;
}


void setup() {
    set_brief("Attic");
    set_long( "You can leave this boring and spartan attic via the stairs in the corner." );
    set_objects( ([
	STAIRS : ({ 1, 0, "/domains/std/Wizroom", 1 }),
	"/domains/std/magic_flame" : 1
      ]) );
}
