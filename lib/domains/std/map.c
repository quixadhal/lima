/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_READABLE;

void setup() {
    set_id("map");
    set_in_room_desc("A map of Lima Bean is pinned to the wall.");
    set_long("It has a crude map of Lima Bean written on it.\n");
    set_text(@FOO
             MORTAL AREA   ATTIC
                  |       /
                  |      /  QUIET ROOM
    LAVA CAVE     |     /  /
             \    |    /  /       PLAINS
              \   |  U/  /           |
  ELEVATOR -- WIZARD HALL ---- EXAMPLE ROOM -- CAR WASH
             /D   |                  |
            /     |                  |
GENERAL STORE     |          RELATIONS & TOYS  -- ENVIRONMENTAL ROOM
               MONSTER ROOM
FOO);
}

mapping lpscript_attributes() {
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
