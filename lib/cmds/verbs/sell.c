/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_sell_obj_to_liv(object ob, object liv) {
    liv->buy_object(ob);
}

mixed *query_verb_info() {
    return ({ ({ "OBJ to LIV" }) });
}
