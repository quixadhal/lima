/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed can_verb_rule() {
    return check_vision();
}

void do_write_str_on_obj(string str, object ob) {
    ob->add_text(str);
    write("Done.\n");
}

void do_write_obj_with_str(object ob, string str) {
    ob->add_text(str);
    write("Done.\n");
}

array query_verb_info()
{
    return ({ ({ "STR on OBJ", "OBJ with STR" }), ({ "inscribe" }) });
}
