/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_WIELDABLE;
inherit M_GETTABLE;
inherit M_MESSAGES;

void create() {
    ::create();
    add_id("weapon");
    set_gettable(1);
}
     
void remove() {
    object::remove();
    wieldable::remove();
}	    
