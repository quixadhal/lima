/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

void clean_object(object ob) {
  int i;
  object *obs;

  obs = all_inventory(ob);
  i = sizeof(obs);

  while (i--) {
    if (obs[i]->is_living()) continue;
    if (first_inventory(obs[i])) clean_object(obs[i]);
    destruct(obs[i]);
  }
}

private void main(mixed *arg) {
    if (!arg[0]) arg[0] = environment(this_body());
    clean_object(arg[0]);
    this_body()->do_player_message("clean");
    return;
}
