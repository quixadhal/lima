/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

array query_verb_info() {
    return ({ ({ "OBJ" }) });
}

void do_ring_obj(object ob) {
    ob->ring();
}
