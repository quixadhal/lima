/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_knock_on_obj(object o, string s)
{
  o->knock_knock(s);
}

void do_knock_at_obj(object o, string s)
{
  o->knock_knock(s);
}

mixed *query_verb_info() {
    return ({ ({ "on OBJ", "at OBJ" }), ({ "rap" }) });
}

