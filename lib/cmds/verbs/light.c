/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_light_obj(object ob) {
    ob->do_light();
}

void do_light_obj_with_obj(object ob, object with) {
    ob->do_light(with);
}

mixed * query_verb_info() {
    return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
