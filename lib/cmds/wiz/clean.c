/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: dest, clone
//USAGE:  clean
//        clean <target>
//
//This command will destroy everything in your environment if
//executed with no args. If given an argument it will destroy all
//objects in that object.  Especially useful when a pesky wizard or two
//clone 20 barney's in your workroom.
//It ignores add_items and living objects, so perhaps it's not so helpful
//with those barneys after all ...

inherit CMD;

void clean_object(object ob)
{
  int i;
  object *obs;

  obs = all_inventory(ob);
  i = sizeof(obs);

  while (i--)
  {
    if (obs[i]->is_living())
      continue;
    if(base_name(obs[i]) == SIMPLE_OB)
      continue;
    if (first_inventory(obs[i]))
      clean_object(obs[i]);
    destruct(obs[i]);
  }
}

private void main(mixed *arg)
{
  if (!arg[0])
    arg[0] = environment(this_body());
  clean_object(arg[0]);
  this_body()->do_player_message("clean");
  return;
}
