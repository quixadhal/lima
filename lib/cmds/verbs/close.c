/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed do_close_obj(object ob)
{
    ob->do_close();
}

void create() {
    add_rules( ({ "OBJ" }) );
}
