/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_load_obj_with_obj(object ob1, object ob2)
{
  ob1->do_load(ob2);
}

void do_load_obj_with_obs( object ob1, object array ob2)
{
  handle_obs(ob1, (: do_load_obj_with_obj :), ob2);
}

void do_load_obj_in_obj(object ob1, object ob2)
{
  ob2->do_load(ob1);
}

void do_load_obs_in_obj(object array ob1, object ob2)
{
  handle_obs(ob1, (: do_load_obj_in_obj :), ob2);
}

void create()
{
  add_rules( ({  "OBJ with OBJ", "OBJ with OBS" }), ({}) );
  add_rules( ({ "OBJ in OBJ", "OBS in OBJ" }), ({ "ready" }) );
}
