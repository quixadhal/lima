/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_READABLE;

void setup() {
    set_id("map");
    set_untouched_desc("A map of the academy is pinned to the wall.");
    set_in_room_desc( "A map of the academy has been discarded here.");
    set_long("It has a crude map of the academy drawn on it.");
    set_text(@FOO
                |-----------|----------|
                |           |          |
                | Corridor9 | Problems |
                |           |          |
    |-----------|-----------|----------|
    |   Zoo     |           |
    |  Monsters | Corridor8 |
    |of the mud |           |
    |-----------|-----------|----------|
                |           |  Pantry  |
                | Corridor7 | Food and |
                |           |  drinks  |
    |-----------|-----------|----------|
    | Weaponry  |           |          |
    |-----------| Corridor6 | Emoting  |
    |  Armory   |           | Tutorial |
    |-----------|-----------|----------|
    |           |           |
    |  Objects  | Corridor5 |
    |           |           |
    |-----------|-----------|-------------------|
                |           |      Rooms        |
                | Corridor4 |chats, basic room  | 
                |           |add_item, add_exits|
                |           |set_long, includes |
    |-----------|-----------|-------------------|
    |    Areas  |           |
    |Setting up | Corridor3 |
    | Standards |           |
    |-----------|-----------|----------|
    |           |           |
    |  Unix     | Corridor2 |
    |           |           | 
    |-----------|-----------|----------|
                |           |    ED    |
                | Corridor1 | tutorial |
                |           |          |
    |-----------|-----------|----------|
    |           ||---------||
    |   Newwiz  || Academy ||
    |           ||---------||
    |-----------|-----------|
FOO);
}

mapping lpscript_attributes() {
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
