/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/object";

void setup();
mixed can_light_the_magic_torch();
mixed indirect_light_obj_with_obj();

void setup() {
function f;
set_adj("blue");
set_id("flame");
add_id("fire");
set_in_room_desc("A blue flame hovers in the air in the center of the room.");
set_long("The air itself seems to burn here, as if by magic.");
}


mixed can_light_the_magic_torch() {
{
return 1;;
}
;
}


mixed indirect_light_obj_with_obj() {
{
return 1;;
}
;
}

