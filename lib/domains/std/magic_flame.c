/* Do not remove the headers from this file! see /USAGE for more info. */

inherit FLAME;

void setup() {
    set_adj("blue");
    set_in_room_desc("A blue flame hovers in the air in the center of the room.");
    set_long("The air itself seems to burn here, as if by magic.");
}

int can_light_the_magic_torch() {
    return 1;
}
