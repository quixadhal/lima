/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

mixed can_flip_obj_wrd_obj(object ob1, string p, object ob2) {
    if (p == "for" || p == "to") return default_checks();
    return 0;
}

//### huh?  This makes this useless.
mixed can_flip_on_obj_with_obj(object ob) {
    return 0;
}

void do_flip_obj_for_obj(object ob1, object ob2) {
    write("### Not implemented\n");
}

void do_flip_obj_to_obj(object ob1, object ob2) {
    write("### Not implemented\n");
} 

void do_flip_on_obj_with_obj(object ob1, object ob2) {
    ob1->turn_on(ob2);
} 

void do_flip_obj_with_obj(object ob1, object ob2) {
    ob1->flip(ob2);
} 

void do_flip_off_obj(object ob) {
    ob->turn_off();
}

void do_flip_on_obj(object ob) {
    ob->turn_on();
}

void do_flip_obj_off(object ob) {
    ob->turn_off();
}

void do_flip_obj_on(object ob) {
    ob->turn_on();
}

void do_flip_obj(object ob) {
    ob->flip();
}

array query_verb_info()
{
    return ({ ({ "OBJ", "OBJ for OBJ","OBJ to OBJ","off OBJ","on OBJ with OBJ","OBJ off","OBJ with OBJ","OBJ on","on OBJ" }), ({ "turn" }) });
}
