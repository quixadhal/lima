/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;


void do_knock_on_obj( object obj )
{
    obj->do_knock();
}

void do_knock_at_obj(object obj)
{
    obj->do_knock();
}

array query_verb_info() {
    return ({ ({ "on OBJ", "at OBJ" }), ({ "rap" }) });
}

