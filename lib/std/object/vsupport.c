/* Do not remove the headers from this file! see /USAGE for more info. */


/* Do not remove the headers from this file! see /USAGE for more info. */

#include <hooks.h>
inherit M_PARSING;

int is_visible();
string short();
string is_relation_alias(string al);
int valid_relation(string relation);

varargs mixed call_hooks(string, int, mixed);

//:FUNCTION check_permission
//check_permission(what) calls who->allow(what) in our owner, if one exists.
//Used by many of the verb routines to keep players from fiddling with things
//in other player's/monster's inventories.
mixed check_permission(string what) {
    object who = owner(this_object());

    if (!who || who->allow(what))
	return 1;
    return "#Rather impolite to do that without asking, don't you think?\n";
}

//:FUNCTION default_object_checks
//Nearly all direct/indirect parsing functions should call this.  It checks to 
//make sure that the object is visible to the enactor, amongst other things.
int default_object_checks()
{
  if(!is_visible())
    return 0;
  if(!short())
    return 0;
  return 1;
}
  
//:FUNCTION direct_verb_rule
//The default method of handling direct objects with verbs.
mixed direct_verb_rule(string verb,string rule,mixed args...)
{
  mixed temp;
  if(this_object()->is_container())
  {
    string aliased_to;
    if(rule=="WRD OBJ")
    {
      args[0]=PREPOSITION_D->translate_preposition(args[0]);
      aliased_to=is_relation_alias(args[0]);
      if(!valid_relation(args[0]))
      {
        if(!aliased_to)
          return 0;
        args[0]=aliased_to;
	    }
    }
  } else if(this_object()->is_exit()){
    temp=this_object()->complex_exit_direct_verb_rule(verb,rule,args...);
    if(!temp||stringp(temp))
      return temp;
  } else {
    return 0;
  }
  return default_object_checks();
}

//:FUNCTION do_verb_rule
//The default handling for all verbs.
int do_verb_rule(string verb,string rule,mixed args...)
{
  if(this_object()->is_container())
  {
    string aliased_to;
    if(rule=="WRD OBJ")
    {
      args[0]=PREPOSITION_D->translate_preposition(args[0]);
      aliased_to=is_relation_alias(args[0]);
      if(!valid_relation(args[0]))
	    {
        args[0]=aliased_to;
      }
    }
  }
  if(this_object()->is_exit())
    this_object()->complex_exit_do_verb_rule(verb,rule,args...);
}

//:FUNCTION direct_get_obj
//Do some checks for the parser when we are the OBJ of the "get OBJ" rule
mixed direct_get_obj(object ob)
{
    object who = owner(ob);

    if ( environment() == this_body() )
	return "#You already have it!\n";
    if ( who && who != this_body() )
	return "#Too bad you're not a skilled pickpocket.\n";
    if ( this_object() == this_body() )
	return "#You make an advance on yourself.\n";
    if (environment(this_object())!=environment(this_body()))
      return "#You cannot get that.";
    if(!default_object_checks())
      return 0;

    //:HOOK prevent_get
    //A yes/no/error hook called by direct_get_obj() if the standard conditions
    //succeed
    return call_hooks("prevent_get", HOOK_YES_NO_ERROR);
}

//:FUNCTION direct_get_obj_from_obj
//Handle parser checks for "get OBJ from OBJ"
//From doesn't care what relation Object 1 is in.
mixed direct_get_obj_from_obj(object ob1, object ob2)
{
  if (ob2 != 0 && environment(ob1) != ob2) return 0;
  return call_hooks("prevent_get", HOOK_YES_NO_ERROR);
}

//:FUNCTION direct_get_obj_from_wrd_obj
//Handle parser checks for "get OBJ from WRD OBJ"
//Leave the relation checks to indirect_
mixed direct_get_obj_from_wrd_obj(object ob1, string rel, object ob2)
{
  return direct_get_obj_from_obj(ob1, ob2);
}

//:FUNCTION direct_put_obj_wrd_obj
//Handle parser checks for "put OBJ WRD OBJ"     
mixed direct_put_obj_wrd_obj(object ob1, object ob2)
{
  return check_permission("put");
}

//:FUNCTION direct_get_obj_with_obj
//Handle parser checks for "get OBJ with OBJ"
mixed direct_get_obj_with_obj(object ob1, object ob2) {
    return direct_get_obj(ob1);
}

//:FUNCTION need_to_have
//Do some sanity checks for verbs that auto-take objects, or only allow
//you to use objects you are carrying.
//Most of the work is done in try_to_acquire.
mixed need_to_have() {
  mixed res = direct_get_obj(this_object());
  if(!default_object_checks())
    return 0;
  if (res == "#You already have it!\n")
    return 1;
  return res;
}

//:FUNCTION direct_look_at_obj
//Whether or not an object can be looked at.  If there's no short description, 
//or it's not visible to the looker, return 0.
mixed direct_look_at_obj(object ob) {
  return default_object_checks();
}

//:FUNCTION direct_look_obj
//Whether or not an object can be looked at.  If there's no short description, 
//or it's not visible to the looker, return 0.
mixed direct_look_obj(object ob) {
  return default_object_checks();
}

//:FUNCTION direct_look_for_obj
//Always allow "look for OBJ" to succeed by default.
mixed direct_look_for_obj(object ob) {
    return 1;
}

//:FUNCTION direct_look_wrd_obj
//Set "look WRD OBJ" to fail by default.
mixed direct_look_wrd_obj(object ob) {
    return "#You can't look there";
}

//:FUNCTION direct_sell_obj
//Handle parser checks for "sell OBJ"
mixed direct_sell_obj(object ob) {
  return need_to_have();
}

//:FUNCTION direct_smell_obj
//Handle parser checks for "smell OBJ" rule.
mixed direct_smell_obj(object ob)
{
    if(!default_object_checks())
      return 0;
    return check_permission("smell");
}

//:FUNCTION direct_give_obj_to_liv
//Handle parser checks for "give OBJ to LIV" rule, where we are the object
//being given.
mixed direct_give_obj_to_liv( object obj, object liv )
{
  return need_to_have();
}

//:FUNCTION direct_eat_obj
//Handle parser checks for "eat OBJ" rule.
mixed direct_eat_obj(object ob) {
    if(!default_object_checks())
      return 0;
    return "I don't think " + this_object()->the_short() + " would agree with you.\n";
}

//### shouldn't these to only be in coins?
mixed direct_give_wrd_str_to_liv()
{
    return 1;
}

//### This DEFINATELY shouldn't be here.  Should be in living.c
mixed indirect_give_wrd_str_to_liv()
{
    return 1;
}

mixed direct_look_str_obj(string prep, object ob) {
    return "There is nothing " + prep + " " + this_object()->the_short() + ".\n";
}



//:FUNCTION direct_sell_obj_to_liv
//Handle parser checks for "sell OBJ to LIV"
mixed direct_sell_obj_to_liv(object ob, object liv, mixed foo) {
    return need_to_have();
}

//:FUNCTION direct_buy_obj_from_liv
//Handle parser checks for "buy OBJ from LIV"
mixed direct_buy_obj_from_liv(object ob, object liv) {
    if (owner(ob) != liv)
	return liv->query_possessive() + "doesn't have that!\n";
    if(!default_object_checks())
      return 0;
    return 1;
}

//:FUNCTION direct_drop_obj
//Handle parser checks for "drop OBJ" 
mixed direct_drop_obj(object ob)
{
    if(!default_object_checks())
      return 0;
    if ( environment() != this_body() )
	return "#You don't have it!\n";

    //:HOOK prevent_drop
    //A yes/no/error hook called by direct_drop_obj() if the standard conditions
    //succeed

    return call_hooks("prevent_drop", HOOK_YES_NO_ERROR);
}

//:FUNCTION direct_flip_obj
//Handle parser checks for "flip OBJ"
mixed direct_flip_obj(object ob) {
  if(!default_object_checks())
    return 0;
  //:HOOK direct_flip
  //A yes/no/error hook called by direct_flip_obj(); if no hooks exists, the
  //useless message will be used.
  
  return call_hooks("direct_flip", HOOK_YES_NO_ERROR,
		    useless( "Fiddling with " + ob->the_short()));
}

//:FUNCTION direct_throw_obj
//Handle parser checks for "throw OBJ"
mixed direct_throw_obj(object ob1, object ob2)
{
    return need_to_have();
}

//:FUNCTION direct_throw_obj_at_obj
//Handle parser checks for "throw OBJ at OBJ"
mixed direct_throw_obj_at_obj(object ob1, object ob2)
{
    return need_to_have();
}

//:FUNCTION indirect_throw_obj_at_obj
//Handle parser checks for "throw OBJ at OBJ"
mixed indirect_throw_obj_at_obj(object ob1, object ob2)
{
  return default_object_checks();
}

//:FUNCTION direct_pull_obj
// Handle parser checks for "pull OBJ"
mixed direct_pull_obj( object ob )
{
    if(!default_object_checks())
      return 0;
    return useless( "Pulling " + ob->the_short() );
}

//:FUNCTION direct_press_obj
// Parser check for "press OBJ"
mixed direct_press_obj( object ob )
{
    if(!default_object_checks())
      return 0;
    return useless( "Pushing " + ob->the_short());
}

//:FUNCTION direct_search_obj
// Parser check for "search OBJ"
mixed direct_search_obj( object ob )
{
  return default_object_checks();
}


//:FUNCTION direct_search_obj_for_obj
//Default
mixed direct_search_obj_for_obj( object ob1, object ob2 )
{
  return default_object_checks();
}

//:FUNCTION indirect_search_obj_for_obj
//Default
mixed indirect_search_obj_for_obj( object ob1, object ob2 )
{
  return default_object_checks();
}

//FUNCTION direct_search_for_str
//Default
mixed direct_search_for_str( string str )
{
    return 1;
}

//:FUNCTION indirect_search_obj_with_obj
//Default.
mixed indirect_search_obj_with_obj( object ob1, object ob2 )
{
  if(!default_object_checks())
      return 0;
    if( environment(ob2) != this_body())
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//:FUNCTION direct_search_obj_with_obj
//Default
mixed direct_search_obj_with_obj( object ob1, object ob2 )
{
    if(!default_object_checks())
      return 0;
    return 1;
}

//:FUNCTION direct_search_for_str_in_obj
//Default
mixed direct_search_for_str_in_obj( string str, object ob )
{
    return 1;
}

//:FUNCTION direct_search_obj_for_str
//Default
mixed direct_search_obj_for_str( object ob, string str )
{
    if(!default_object_checks())
      return 0;
    return 1;
}

//:FUNCTION direct_search_obj_with_obj_for_str
//Default
mixed direct_search_obj_with_obj_for_str( object ob, string str )
{
    if(!default_object_checks())
      return 0;
    return 1;
}

//:FUNCTION indirect_search_obj_with_obj_for_str
mixed indirect_search_obj_with_obj_for_str( object ob1, object ob2,  string str )
{
    if(!default_object_checks())
      return 0;
    if( environment(ob2) != this_body())
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//:FUNCTION direct_search_for_str_in_obj_with_obj
//Default
mixed direct_search_for_str_in_obj_with_obj( string str, object ob1, object ob2 )
{
    return 1;
}

//:FUNCTION indirect_search_for_str_in_obj_with_obj
//Default 
mixed indirect_search_for_str_in_obj_with_obj( string str, object ob1, object ob2 )
{
    if(!default_object_checks())
      return 0;
    if( environment( ob2 ) != this_body() )
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//:FUNCTION direct_search_obj_for_str_with_obj
//Default
mixed direct_search_obj_for_str_with_obj( object ob1, string str, object ob2 )
{
    if(!default_object_checks())
      return 0;
    return 1;
}

//:FUNCTION indirect_search_obj_for_str_with_obj
//Default
mixed indirect_search_obj_for_str_with_obj( object ob1, string str, object ob2 )
{
    if(!default_object_checks())
      return 0;
    if( environment( this_object()) != this_body())
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//FUNCTION do_search
// Default searching.
varargs void do_search( object with, string search_for )
{
    string str = "$N $vsearch ";
    if( this_object() == environment( this_body()))
	str += "around";
    else str += "the $o0";
    if( with ) str += " with $p $o1";
    str += ", but $vdiscover nothing.";
    this_body()->simple_action( str, this_object(), with );
}


//:FUNCTION direct_listen_to_obj
//Default
mixed direct_listen_to_obj( object obj )
{
    if(!default_object_checks())
      return 0;
    return "You hear nothing special.";
}
