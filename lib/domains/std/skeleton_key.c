/* Do not remove the headers from this file! see /USAGE for more info. */

    inherit KEY;

void setup()
{
    set_adj("small", "shiny");
    set_in_room_desc("A small shiny safe key");
    set_long("This is a small shiny safe key used to open a safe.");
}

mixed key_type()
{
    return "skeleton";
}

