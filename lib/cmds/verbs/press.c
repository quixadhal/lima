/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_press_obj(object ob, string name) {
    ob->do_press();
}

void do_press_obj_with_obj(object ob1, object ob2) {
    ob1->do_press( ob2 );
}

void do_press_obj_str(object ob, string str) {
    ob->do_press( str );
}

void create() {
    add_rules( ({ "OBJ", "OBJ STR", "OBJ with OBJ" }), ({ "push" }) );
}
