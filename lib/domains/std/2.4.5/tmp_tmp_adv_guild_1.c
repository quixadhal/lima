/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/std/book";
inherit "/std/modules/m_gettable";

void setup();

void setup() {
function f;
set_flag(0 | ATTACHED);
add_id("book");
set_getmsg("The book is attached to the chain.");
set_in_room_desc("a book on a chain");
set_long("There is a book hanging on a chain from the wall.\n The title is: 'ADVENTURING'.");
set_text("/domains/std/2.4.5/book_text");
}

