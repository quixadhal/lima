/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Daemon which serves the purpose of translating synonyms for "go methods"
 * on exits.  
 * Written by Tigran@Illusions, Lima Bean   Feb 4, 1999
 */

inherit M_DAEMON_DATA;

void save_me();

mapping methods=([]);

//:FUNCTION add_method()
//Add a method and a set of equivalents
//Any arguments after the first are equivalents to the method.
void add_method(string method,string array equivs...)
{
  string array add=({});
  string verb,prep;
  if(!check_privilege("Mudlib:daemons"))
    error("Insufficient privileges to add_method()");
  if(sscanf(method,"%s %s",verb,prep)==2)
    {
      /* There is a preposition */
      prep=PREPOSITION_D->translate_preposition(prep);
      if(member_array(prep,PREPOSITION_D->consolidated_preps())==-1)
	error("Illegal preposition in method");
      method=sprintf("%s %s",verb,prep);
    }
  equivs=clean_array(({method})+equivs);
  foreach(string test in equivs)
    {
      if(sscanf(test,"%s %s",verb,prep)==2)
	{
	  /* There is a preposition */
	  prep=PREPOSITION_D->translate_preposition(prep);
	  if(member_array(prep,PREPOSITION_D->consolidated_preps())==-1)
	    error("Illegal preposition in method");
	  add+=({sprintf("%s %s",verb,prep)});
	}
      add+=({test});
    }
  methods[method]=clean_array(add);
  save_me();
}

//:FUNCTION remove_method()
//Remove a method and it's equivalents
void remove_method(string method)
{
  if(!check_privilege("Mudlib:daemons"))
    error("Insufficient privileges to remove_method");
  map_delete(methods,method);
  save_me();
}

//:FUNCTION list_methods()
//Return an array of all methods which have equivalents
string array list_methods()
{
  return keys(methods);
}

//:FUNCTION add_method_equivalants()
//Add an additional equivalents to a given method.
void add_method_equivalents(string method,string array equivs...)
{
  string array add=({});
  if(!check_privilege("Mudlib:daemons"))
    error("Insufficient privileges to add_method_equivalents");
  if(member_array(method,keys(methods))==-1)
    error("Attempted to add equivalents to a nonexistant method");  
  foreach(string test in equivs)
    {
      string verb,prep;
      if(sscanf(test,"%s %s",verb,prep)==2)
	{
	  /* There is a preposition */
	  prep=PREPOSITION_D->translate_preposition(prep);
	  if(member_array(prep,PREPOSITION_D->consolidated_preps())==-1)
	    error("Illegal preposition in method");
	  add+=({sprintf("%s %s",verb,prep)});
	}
      add+=({test});
    }
  
  methods[method]=clean_array(add+methods[method]);
  save_me();
}

//:FUNCTION remove_method_equivalents()
//Remove equivalents from a given method
void remove_method_equivalents(string method, string array equivs...)
{
  if(!check_privilege("Mudlib:daemons"))
    error("Insufficient privileges to remove_method_equivalents");
  if(member_array(method,keys(methods))==-1)
    error("Attempted to remove equivalents from a nonexistant method");
  methods[method]-=equivs;
  if(!sizeof(methods[method]))
    remove_method(method);
  save_me();
}

//:FUNCTION list_method_equivalents()
//Return an array of equivalents to a given method
string array list_method_equivalents(string method)
{
  if(member_array(method,keys(methods))==-1)
    return 0;
  return methods[method];
}
     
mapping debug()
{
  return methods;
}
