/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_WEARABLE;
inherit M_GETTABLE;
inherit M_DAMAGE_SINK;

void create() {
    ::create();
    add_id("armor");
    set_gettable(1);
}

void set_is_on(int g) {
    if (g) environment()->add_armor(this_object());
    else environment()->remove_armor(this_object());
    ::set_is_on(g);
}
