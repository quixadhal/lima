/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;
inherit M_WEARABLE;
inherit M_VALUABLE;

void setup() {
    set_adj("red");
    set_id("scarf");
    set_gettable(1);
set_value(10);
}
