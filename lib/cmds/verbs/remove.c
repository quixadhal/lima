/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_remove_obj(object ob)
{
    ob->do_remove();
}

mixed *query_verb_info() {
    return ({ ({ "OBJ" }), ({ "take off" }) });
}
