/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_extinguish_obj(object ob) {
    ob->do_extinguish();
}

void create()
{
    add_rules( ({ "OBJ" }) );
}
