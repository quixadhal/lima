/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ARMOR;

create() {
    ::create();
    add_id(({ "shirt", "chainmail" }));
    add_adj("chainmail");
    set_armor_class(4);
    set_resist("blade", 2); // a little better against blades
}
