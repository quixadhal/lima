/* Do not remove the headers from this file! see /USAGE for more info. */

#include <hooks.h>
inherit M_PARSING;


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

    //:HOOK prevent_get
    //A yes/no/error hook called by direct_get_obj() if the standard conditions
    //succeed

    return call_hooks("prevent_get", HOOK_YES_NO_ERROR);
}

//:FUNCTION need_to_have
//Do some sanity checks for verbs that auto-take objects, or only allow
//you to use objects you are carrying.
//Most of the work is done in try_to_acquire.
mixed need_to_have() {
    mixed res = direct_get_obj(this_object());

    if (res == "#You already have it!\n")
	return 1;
    return res;
}

//:FUNCTION direct_look_at_obj
//Always allow "look at OBJ" to succeed by default.
mixed direct_look_at_obj(object ob) {
    return 1;
}

//:FUNCTION direct_look_at_obj
//Always allow "look OBJ" to succeed by default.  This is actually used to
//support "examine OBJ".
mixed direct_look_obj(object ob) {
    return 1;
}

//:FUNCTION direct_look_at_obj
//Always allow "look for OBJ" to succeed by default.
mixed direct_look_for_obj(object ob) {
    return 1;
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

//:FUNCTION direct_put_obj_wrd_obj
//Handle parser checks for "put OBJ WRD OBJ"     
mixed direct_put_obj_wrd_obj(object ob1, object ob2) {
    return check_permission("put");
}

//:FUNCTION direct_get_obs
//Handle parser checks for "get OBS"
mixed direct_get_obs(object ob)
{
    if ( environment() == this_body() || 
      (environment() && environment()->is_living()) ||
      this_object() == this_body() )
	return 0;

    //:HOOK prevent_get
    //A yes/no/error hook called by direct_get_obj() if the standard conditions
    //succeed

    return call_hooks("prevent_get", HOOK_YES_NO_ERROR);
}

//:FUNCTION direct_get_obj_from_obj
//Handle parser checks for "get OBJ from OBJ"
mixed direct_get_obj_from_obj(object ob1, object ob2) {
    if (ob2 != 0 && environment(ob1) != ob2) return 0;
    return direct_get_obj(ob1);
}

//:FUNCTION direct_get_obj_out_of_obj
//Handle parser checks for "get OBJ out of OBJ"
mixed direct_get_obj_out_of_obj(object ob1, object ob2) {
    return direct_get_obj(ob1);
}

//:FUNCTION direct_get_obj_with_obj
//Handle parser checks for "get OBJ with OBJ"
mixed direct_get_obj_with_obj(object ob1, object ob2) {
    return direct_get_obj(ob1);
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
    return 1;
}

//:FUNCTION direct_drop_obj
//Handle parser checks for "drop OBJ" 
mixed direct_drop_obj(object ob)
{
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
    return 1;
}

//:FUNCTION direct_pull_obj
// Handle parser checks for "pull OBJ"
mixed direct_pull_obj( object ob )
{
    return useless( "Pulling " + ob->the_short() );
}

//:FUNCTION direct_press_obj
// Parser check for "press OBJ"
mixed direct_press_obj( object ob )
{
    return useless( "Pushing " + ob->the_short());
}

//:FUNCTION direct_search_obj
// Parser check for "search OBJ"
mixed direct_search_obj( object ob )
{
    return 1;
}


//:FUNCTION direct_search_obj_for_obj
//Default
mixed direct_search_obj_for_obj( object ob1, object ob2 )
{
    return 1;
}

//:FUNCTION indirect_search_obj_for_obj
//Default
mixed indirect_search_obj_for_obj( object ob1, object ob2 )
{
    return 1;
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
    if( environment(ob2) != this_body())
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//:FUNCTION direct_search_obj_with_obj
//Default
mixed direct_search_obj_with_obj( object ob1, object ob2 )
{
    return 1;
}

//:FUNCTION direct_search_for_str_in_obj( string str, object obj )
//Default
mixed direct_search_for_str_in_obj( string str, object ob )
{
    return 1;
}

//:FUNCTION direct_search_obj_for_str
//Default
mixed direct_search_obj_for_str( object ob, string str )
{
    return 1;
}

//:FUNCTION direct_search_obj_with_obj_for_str
//Default
mixed direct_search_obj_with_obj_for_str( object ob, string str )
{
    return 1;
}

//:FUNCTION indirect_search_obj_with_obj_for_str
mixed indirect_search_obj_with_obj_for_str( object ob1, object ob2,  string str )
{
    if( environment(ob2) != this_body())
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//:FUNCTION
//Default

mixed direct_search_for_str_in_obj_with_obj( string str, object ob1, object ob2 )
{
    return 1;
}

//:FUNCTION indirect_search_for_str_in_obj_with_obj
//Default 
mixed indirect_search_for_str_in_obj_with_obj( string str, object ob1, object ob2 )
{
    if( environment( ob2 ) != this_body() )
	return "Maybe you should search it with something you're holding.";
    return 1;
}

//:FUNCTION direct_search_obj_for_str_with_obj
//Default
mixed direct_search_obj_for_str_with_obj( object ob1, string str, object ob2 )
{
    return 1;
}

//:FUNCTION indirect_search_obj_for_str_with_obj
//Default
mixed indirect_search_obj_for_str_with_obj( object ob1, string str, object ob2 )
{
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
    return "You hear nothing special.";
}
