/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;

private nosave int loaded;
private nosave int counter;
private nosave int time_to_load = 2;

// Treat as a loadable weapon for combat
// ie use the loaded ammunition for messaging etc
int query_loadable() { return 1; }

// Is it loaded ?
int query_loaded(){ return loaded; }

// Called when it is fired - ammunition is moved to firer's environment
// and ammunition records of weapon are updated
void discharge()
{
  loaded = 0;
  counter = time_to_load;
  this_body()->simple_action("$N $vfire the $o.", this_object());
}

void load()
{
  counter --;
  if(counter<1)
  {
    loaded = 1;
    this_body()->simple_action("$N $vfinish loading the $o.", this_object());
  } else
    this_body()->simple_action("$N $vcontinue to load the $o.", this_object());
}

mixed direct_fire_obj_at_liv(object ob, object liv)
{
  if(!sizeof(this_body()->query_readied()))
    return "Need to ready some ammunition first.";
  if(!query_loaded())
    return "Need to load it before you can fire it.";
  return 1;
}

void do_fire_at_liv(object liv)
{
  this_body()->start_fight(liv);
}
