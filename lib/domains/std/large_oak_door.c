/* Do not remove the headers from this file! see /USAGE for more info. */

inherit DOOR;

void setup(string dir) {
    set_id("door");
    set_adj("large oak", "large", "oak");
    set_long("It is about 10 feet tall, and very beautiful.\n");
    set_closed(1);
    setup_door("large oak door", dir);
    set_in_room_desc("A large oak door takes up most of the " + dir + " wall of the room.");
}
