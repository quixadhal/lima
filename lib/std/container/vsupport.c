/* Do not remove the headers from this file! see /USAGE for more info. */

// Verb support for containers

int is_visible();
string short();

mixed basic_checks()
{
  if(!is_visible())
    return "not visible";
  if(!short())
    return "no short";
  if(this_object()->query_closed())
    return "closed";
  return 1;
}

mixed indirect_put_obj_wrd_obj(object ob1, string wrd, object ob2)
{
  return basic_checks();
}

mixed indirect_get_obj_from_obj(object ob1, string wrd, object ob2)
{
  return basic_checks();
}

//:FUNCTION indirect_get_obj_from_wrd_obj
//Handle parser checks for "get OBJ from WRD OBJ"
//From WRD checks what relation object 1 is in.
mixed indirect_get_obj_from_wrd_obj(object ob1, string rel, object ob2)
{
  string where;
  string * aliases;
  if ((ob1 != 0) && (environment(ob1) != ob2))
    return 0;

  if(!ob1)
    return 0;

  where = ob2->query_relation(ob1);
// ADD ERROR CHECK FOR NO RELATION ?

  if(where!=rel)
  {
    aliases=ob2->query_relation_aliases(rel);
    if(!sizeof(aliases) || (member_array(where, aliases) <0))
      return "It's not " + rel + " there.\n";
  }
  return basic_checks();
}

//:FUNCTION indirect_verb_rule
// The default verb handling where the container is an indirect object.  
//mixed indirect_verb_rule(string verb,string rule,mixed second,mixed third,mixed fourth)
//{
//    string aliased_to;
//    third=PREPOSITION_D->translate_preposition(third);
//    // The third should /always/ be the relation.  If this is in err..oops --Tig
//    aliased_to=is_relation_alias(third);
//    //  if(!valid_relation(third))
//    //    {
//    //      if(!aliased_to)
//    //	return 0;
//    //      third=aliased_to;
//    //    }
//    //  if(query_relation(second)!=third)
//    //    return 0;
//    if(!is_visible())
//	return "not visible";
//    if(!short())
//	return "no short";
//    if(this_object()->query_closed()&&third=="in")
//	return "closed";
//    return 1;
//}

