/* Do not remove the headers from this file! see /USAGE for more info. */
// Modified by Uae.  ZorkMUD.  May 21st 1997.

// Try_to_acquire removed from the conditions of drinking
// an object.  drink "" added with fail message "Drink what?".

//  Uae.  May 23rd. 1997.  added:
//  drink_str added.  This is to support drinking from renewable 
//  fluid sources when the fluid_object itself is not present.
//  In your renewable fluid source's id, put "<fluid>_source".

#include <mudlib.h>
#include <setbit.h>

inherit NVERB_OB;

mixed can_drink(){
   return "Drink what?";
}


mixed source_find( string fluid )// Sees if there is, and returns, a fluid source near drinker.
{	
  object source;
  source = present( fluid +"_source", environment( this_body()));
  if (!source) return 0;
  if (source -> id( "drink_source" )) return source; 
  else return "Try putting the "+ fluid + " into something.\n";
}


mixed can_drink_str( string fluid ){// Is the character in the presence of a fluid source?
  return source_find(fluid);
}

void do_drink_str( string fluid ){
  source_find(fluid) -> drink_from_it( fluid );
}

mixed can_drink_str_from_obj( string fluid ){
  return source_find(fluid);
}

int can_drink_obj_from_obj(){ return 1;}

void do_drink_str_from_obj( string fluid ){
  source_find(fluid) -> drink_from_it( fluid );
}

void do_drink_obj(object ob)
{
    ob->drink_it();
}

void do_drink_from_obj(object ob)
{
   ob->drink_from_it( 0 );
}

void do_drink_obj_from_obj( object ob1, object ob2 )
{
  ob2 -> drink_from_it( ob1);
}

void create()
{
      add_rules( ({ "", "OBJ",  "STR", "OBJ from OBJ", "from OBJ",  "STR from OBJ" }) );
}

