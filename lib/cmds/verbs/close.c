/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

mixed do_close_obj(object ob)
{
    ob->close();
}

void create() {
    add_rules( ({ "OBJ" }) );
}
