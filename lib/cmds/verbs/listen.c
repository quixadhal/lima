/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_listen_to_obj(object ob)
{
    ob->do_listen();
}


void do_listen()
{
    environment(this_body())->do_listen();
}

void create()
{
    add_rules( ({ "", "to OBJ" }) );
}
