/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_climb_obj(object ob) {
    ob->climb();
}

void do_climb_up_obj(object ob) {
    ob->climb("up");
}

void do_climb_down_obj(object ob) {
    ob->climb("down");
}

mixed * query_verb_info() {
    return ({ ({ "OBJ", "up OBJ", "down OBJ" }) });
}
