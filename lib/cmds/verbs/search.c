/* Do not remove the headers from this file! see /USAGE for more info. */

/* Wraith@Lima Bean / Hatter@Wonderland
   infodawn@lima.mudlib.org

   Verb to search for things that may be hidden.
*/

inherit VERB_OB;

mixed direct_search_obj(object ob1) { 
  if (ob1->can_search_me(0, 0)) return 1;
  this_body()->other_action("$N $vsearch around.\n");
  return "#You search around but find nothing useful.\n";
}

mixed direct_search_obj_with_obj(object ob1, object ob2) { 
  if (ob1->can_search_me(0, ob2)) return 1;
  this_body()->other_action("$N $vsearch around.\n");
  return "#You search around but find nothing useful.\n";
}

mixed can_search() {
  if (!environment(this_body())) return "You have no environment.\n";
  if (environment(this_body())->can_search_me(0, 0)) return 1;
  this_body()->other_action("$N $vsearch around.\n");
  return "You search vaguely around, but find nothing interesting.\n";
}

mixed can_search_for_str(string str) {
  if (!environment(this_body())) return "You have no environment.\n";
  if (environment(this_body())->can_search_me(str, 0)) return 1;
  this_body()->other_action("$N $vsearch around.\n");
  return "You can't find "+str+" here.\n";
}

mixed direct_search_for_str_in_obj(string str1, object ob1) { 
  if (ob1->can_search_me(str1, 0)) return 1;
  this_body()->other_action("$N $vsearch around.\n");
  return "#You can't find "+str1+" in that.\n";
}

mixed direct_search_for_str_in_obj_with_obj(string str1, object ob1, object ob2) { 
  if (ob1->can_search_me(str1, ob2)) return 1;
  this_body()->other_action("$N $vsearch around.\n");
  return "#You can't find "+str1+" in that.\n";
}

int need_to_see() { return 1; }

void do_search() { 
  environment(this_body())->search(0, 0); 
  return;
}

void do_search_obj(object ob) {
  ob->search(0, 0);
  ob->set_searched(1);
  return;
}

void do_search_obj_with_obj(object ob1, object ob2) {
  ob1->search(ob2, 0);
  ob1->set_searched(1);
  return;
}

void do_search_for_str(string str) { 
  environment(this_body())->search(0, str);
  environment(this_body())->set_searched(1);
  return;
}

void do_search_for_str_in_obj(string str, object ob) { 
  ob->search(0, str); 
  ob->set_searched(1);
  return;
}

void do_search_for_str_in_obj_with_obj(string str, object ob1, object ob2) {
  ob1->search(ob2, str); 
  ob1->set_searched(1);
  return;
}

array query_verb_info() {
    return ({ ({ "OBJ", "OBJ with OBJ", "for STR", "for STR in OBJ", 
                 "for STR in OBJ with OBJ", "" }) });
}
