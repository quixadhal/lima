/* Do not remove the headers from this file! see /USAGE for more info. */

#ifdef USE_BODYSLOTS
#include <bodyslots.h>
#endif

inherit OBJ;
inherit M_WEARABLE;
inherit M_GETTABLE;
inherit M_DAMAGE_SINK;

//:MODULE
//This is the base for creating a piece of armor.  It uses M_WEARABLE to
//allow it to be worn, and M_DAMAGE_SINK to allow it to absorb damage.

void mudlib_setup()
{
    ::mudlib_setup();
    add_id("armor");
    set_gettable(1);
}

//:FUNC set_worn
//set_worn(1) causes this object to be worn by whatever is holding it.
//set_worn(0) takes it back off again.
#ifndef USE_BODYSLOTS
void set_worn(int g) {
    if (g) environment()->add_armor(this_object());
    else environment()->remove_armor(this_object());
    ::set_worn(g);
}
#endif

void remove() {
    m_wearable::remove();
    object::remove();
}

mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_damage_sink::lpscript_attributes() + m_gettable::lpscript_attributes();
}
