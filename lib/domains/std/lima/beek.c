/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIVING;
inherit M_CONVERSATION;

void setup() {
    set_name("Beek");
    set_gender(1);
    set_proper_name("Beek");
    set_in_room_desc("Beek is here, fooling around with some piece of code.");
    set_long("You can't tell much about him, and his description (\"This is a test\") isn't notable helpful.\n");
    
    set_options( ([
	"hello" : "Hey there, what's up?",
    ]) );
    set_responses( ([
    ]) );
    set_start( ({ "hello" }) );
}
