/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_enter_obj(object ob, string s)
{
    ob->do_enter(s);
}

mixed *query_verb_info()
{
    return ({ ({ "OBJ" }) });
}
