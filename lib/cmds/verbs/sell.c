/* Do not remove the headers from this file! see /USAGE for more info. */

// Original command by Darklord.
/* Changed to allow default vendor 6-Feb-98 by MonicaS */

inherit VERB_OB;

mixed test_vendor() {
  object ob, vendor;
  foreach(ob in deep_useful_inv(parser_root_environment(environment(this_body())))) {
    if(ob->query_vendor()) {
      if (vendor)
	return "Sell to which vendor?\n";
      vendor = ob;
    }
  }
  if (!vendor) {
    return "There is no vendor here.\n";
  }
  return 1;
}

object find_vendor() {
  foreach(object ob in deep_useful_inv(parser_root_environment(environment(this_body())))) {
    if(ob->query_vendor()) {
      return ob;
    }
  }
  // can't happen, because test_vendor() was called already
  return 0;
}

mixed can_sell_obj() {
  return test_vendor();
}

void do_sell_obj_to_liv(object ob, object vendor) {
  if (!try_to_acquire(ob))
    return;
  vendor->buy_object(ob);
}

void do_sell_obj(object ob) {
  if (!try_to_acquire(ob))
    return;
  find_vendor()->buy_object(ob);
}

void create() {
  add_rules( ({ "OBJ to LIV", "OBJ" }) );
}
