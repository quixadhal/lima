/* Do not remove the headers from this file! see /USAGE for more info. */

inherit PORTAL;

void setup ()
{
    set_flag(ATTACHED);
    set_long("The archway is filled with some sort of a curtain made of "
	     "a shimmering light.  It is set into the southern wall.");
    add_adj("lighted", "shimmering", "southern", "south");
    set_id("archway");
    add_id("portal");
    set_destination(WIZARD_START);
//    set_look_in_desc( "You can vaguely see a sandy beach.");
}
