/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/object";
inherit "/std/modules/m_readable";
inherit "/std/modules/m_gettable";

void setup();

void setup() {
function f;
add_id("list");
set_getmsg("It is firmly attached to the wall.");
set_flag(0 | ATTACHED);
set_long("All the quests are listed on the list.");
set_text("1. Retrieve the Orc slayer from the evil orc shaman, and give it to Leo.");
}

