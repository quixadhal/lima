/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;
inherit M_SEARCHABLE;

void setup() {
    set_area("wiz_area");
    set_brief("Searchable room");
    set_long("This room is filled with junk and rubbish, if you were to search, you would probably find something.");
    searchable_once(1);
}

void do_search(object with, string item) {
    this_body()->simple_action("$N $vsearch around the room and $vfind a "
      "sword buried beneath the junk.\n");
    give_new_obj( this_body(), "/domains/std/objects/sword", 0);
}

void reset()
{
    m_searchable::reset();
    indoor_room::reset();
}
