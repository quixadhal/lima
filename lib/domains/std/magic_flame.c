/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void setup() {
    set_adj("blue");
    set_id("flame", "fire");
    set_in_room_desc("A blue flame hovers in the air in the center of the room.\n");
    set_long("The air itself seems to burn here, as if by magic.\n");
}

int can_light_the_magic_torch() {
    return 1;
}

mixed indirect_light_obj_with_obj() {
    return 1;
}

