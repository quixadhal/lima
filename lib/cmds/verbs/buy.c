/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_buy_obj_from_liv(object ob, object liv) {
    liv->sell_object(ob);
}

mixed *query_verb_info() {
    return ({ ({ "OBJ from LIV" }) });
}
