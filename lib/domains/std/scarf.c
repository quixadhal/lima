/* Do not remove the headers from this file! see /USAGE for more info. */

#ifdef USE_BODYSLOTS
#include <bodyslots.h>
#endif

inherit OBJ;
inherit M_GETTABLE;
inherit M_WEARABLE;
inherit M_VALUABLE;

void setup() {
    set_adj("red");
    set_id("scarf");
#ifdef USE_BODYSLOTS
    set_slot(HEAD);
#endif
    set_value(10000);
    set_size(SMALL);
}
mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
