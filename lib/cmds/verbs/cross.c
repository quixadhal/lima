/* Do not remove the headers from this file! see /USAGE for more info. */


inherit VERB_OB;

void do_cross_word_obj(string prep,object ob)
{
  ob->do_verb_rule("cross","WRD OBJ",prep,ob);
}

void do_cross_obj(object ob)
{
  ob->do_verb_rule("cross","OBJ",ob);
}

void create() {
    add_rules( ({ "OBJ" }) );
}
