/* Do not remove the headers from this file! see /USAGE for more info. */

// This is a portable bulletin board
// Its simple, but since it took me a while to get it working right I
// figured I would save you the trouble.
// Vette  April 16, 1997

inherit BOARD_OB;
inherit M_GETTABLE;


void mudlib_setup(){
    ::mudlib_setup("Portable Bulletin Board", "lima.bugs", 1 );
}

string a_short() {
  return evaluate((: do_desc :));
}
