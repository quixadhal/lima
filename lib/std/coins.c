/* Do not remove the headers from this file! see /USAGE for more info. */




#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;
 
int long_amount;
string long_type;

string long_descr()
{
    string descr = "A pile of " + long_amount + " " + long_type + " coins.\n";
    return descr;
}

void create(int amount, string type)
{
    ::create();
    long_amount = amount;
    long_type = type;

    set_in_room_desc("A pile of coins");
    add_id( "coins",  "coin", "pile", "pile of coins" );
    set_long( (: long_descr :) );

    set_gettable(1);
}

mixed prevent_get()
{
   write ("debugging to see if i get here");
   return 1;
}
