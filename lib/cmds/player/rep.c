/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_MESSAGES;



private void main(string notused)
{
int hp = this_body()->query_hp();
int sp = this_body()->query_sp();
 
this_body()->simple_action("$N $vreport:  HP: " + hp + "  SP: " + sp + ".\n");
}
