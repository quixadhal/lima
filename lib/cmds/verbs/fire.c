/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_fire_obj(object ob)
{
  ob->do_fire();
}

void do_fire_obs( object array obs)
{
  handle_obs(obs, (: do_fire_obj :));
}

void do_fire_obj_at_liv(object ob, object liv)
{
  ob->do_fire_at(liv);
}

void do_fire_obj_obj(object ob1, object ob2)
{
  ob1->do_fire_exit(ob2);
}

void create()
{
  add_rules( ({ "OBS", "OBS at LIV", "OBJ OBJ" }), ({}) );
}
