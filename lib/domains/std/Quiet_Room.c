/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;

void setup()
{
    set_area("wiz_area");
    set_brief("Quiet Room");
    set_long("This small and neat place is the Quiet Room. Originally intended to be a place where wizards can come to read the newspapers and do other tasks without all the distractions of the Grand Hall, due to the invincible stubbornness of human nature, it has become the spot for many a holy war. Still, if a person sits here in the early hours of the morning, it is still relatively quiet and peaceful.");
    set_objects( ([
	"/domains/std/large_oak_door" : ({ "southwest", "Wizroom" }),
      ]));
}
