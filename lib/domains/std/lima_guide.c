/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit BOOK;
inherit M_GETTABLE;


void setup()
{
    set_id("book", "guide");
    set_proper_name("the LIMA Guide Book");
    set_gettable(1);
    set_closed(1);
    set_in_room_desc("There is a very ornately bound book here.");
    set_long("The cover is carved with an intricate pattern inlaid with gold, at the very top of the front cover is a single word, that appears to be pulsing with a life of its own, LIMA.");
    set_text("This book will at some point contain some interesting bits of information about the LIMA mudlib.  However at this time, much like the documentation of the lib itself, it is unfinished.");
}
