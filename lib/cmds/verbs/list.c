/* Do not remove the headers from this file! see /USAGE for more info. */

// Original command created by Darklord. April, 1997.
/* Changed from command to verb 6-Feb-98 by MonicaS */

inherit VERB_OB;

mixed test_vendor() {
  object ob, vendor;
  foreach(ob in deep_useful_inv(parser_root_environment(environment(this_body())))) {
    if(ob->query_vendor()) {
      if (vendor)
	return "List from which vendor?\n";
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

mixed can_list() {
  return test_vendor();
}

mixed can_list_str() {
  return test_vendor();
}

void do_list() {
  find_vendor()->query_items("all", 0);  
}

void do_list_str(string item) {
  find_vendor()->query_items(item, 0);  
}

void do_list_from_liv(object vendor) {
  vendor->query_items("all", 0);
}
  
void do_list_str_from_liv(string item, object vendor) {
  vendor->query_items(item, 0);
}
  
void create() {
    add_rules( ({ "", "from LIV", "STR", "STR from LIV" }) );
}
