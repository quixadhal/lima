/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_DRINK_CONTAINER;
inherit M_DRINKABLE;

void setup()
{
    set_id("bottle", "flask");
add_adj("clear");
    set_long("Appears to be a normal looking bottle.");
    set_num_drinks(5);
    set_drink_action( "$N $vtake a drink from the bottle.");
    set_last_drink_action( "$N $vtake a drink from the bottle, finishing it off." );
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
