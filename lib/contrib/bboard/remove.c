/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_remove_obj(object ob)
{
    ob->do_remove();
}

void do_remove_str_from_obj(string str, object ob) {
	ob->do_remove(str);
}

array query_verb_info() {
    return ({ ({ "OBJ", "STR from OBJ" }), ({ "take off", "unwield" }) });
}
