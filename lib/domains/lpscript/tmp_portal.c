/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/portal";

void setup();

void setup() {
function f;
set_flag(0 | ATTACHED);
set_long("It's a glowing portal, set into the northerm wall.");
add_adj("glowing", "northern", "north");
set_id("portal");
set_look_in_desc("You can vaguely see a sandy beach.");
{
set_destination(START)
;
}
;
}

