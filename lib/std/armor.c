/* Do not remove the headers from this file! see /USAGE for more info. */

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

//:FUNC set_is_on
//set_is_on(1) causes this object to be worn by whatever is holding it.
//set_is_on(0) takes it back off again.
void set_is_on(int g) {
    if (g) environment()->add_armor(this_object());
    else environment()->remove_armor(this_object());
    ::set_is_on(g);
}
