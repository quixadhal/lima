/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_ring_obj(object ob) {
    ob->ring();
}

void create() {
    add_rules( ({ "OBJ" }) );
}
