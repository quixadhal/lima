/* Do not remove the headers from this file! see /USAGE for more info. */


inherit VERB_OB;

void do_pull_obj(object ob, string name) {
    ob->pull(name);
}

void do_pull_obj_with_obj(object ob1, object ob2) {
    ob2->use("pull", ob1);
}

void do_pull_obj_str(object ob, string str) {
    ob->pull_str(str);
}

array query_verb_info() {
    return ({ ({ "OBJ", "OBJ STR", "OBJ with OBJ" }), ({ "yank" }) });
}
