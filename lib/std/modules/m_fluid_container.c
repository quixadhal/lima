/* Do not remove the headers from this file! see /USAGE for more info. */

// Modified by Uae.  May 1997.

//:MODULE
//Fluids can never be 'gotten'.
//Most containers leak.  Inherit this module into
//your container so that it will fill and pour.

int query_max_capacity();
string the_short();

mixed leak_action = "$O leaks out of $o1.";
mixed full_action = "The $o1 is too full for $o.";
mixed fluid_only = 1;

void look_me();

#include <move.h>

//:FUNCTION can_hold_fluid
//int can_hold_fluid( mixed fluid_ob )
//When fluid is passed into the fluid
//container, this function is called with
//the fluid object as the argument.  returns
//1 by default, but this function can be
//overridden if this container is limited 
//in the types of fluids it can hold.
varargs int can_hold_fluid( mixed ob ){ return 1; }

//### Incompletelly implemented.
void set_fluid_only( mixed x ){ fluid_only = x; }

mixed query_fluid_only(){ return fluid_only; }

//:FUNCTION set_leak_action
//set_leak_action(x)
// 'x' is evaluated if this
//container cannot hold the fluid
//that is being put into it.
void set_leak_action( mixed x ){ leak_action = x; }

void leak ( object ob )
{
  if (functionp(leak_action))
  {
    evaluate ( leak_action, ob );
    return;
  }

  if (!this_body() )
    ob -> remove();
  this_body() -> simple_action ( leak_action, ob, this_object() );
  ob -> move( environment( this_body() )  );
}

int will_fit( object fluid )
{
  int fluid_size, room_left, load;
  
  if ( first_inventory() )
    load = first_inventory() -> query_size();
#ifdef USE_SIZE
  fluid_size = fluid ->query_size();
#endif
#ifdef USE_MASS
  fluid_size = fluid ->query_mass();
#endif
  room_left = query_max_capacity() - load;
  //  write ("Fluid size:  " + fluid_size + "   Room left:   " + room_left + ".\n");
  return (fluid_size <= room_left );
}

//:FUNCTION set_full_action
//set_full_action(x)
//'x' is evaluated when this container
//is full of fluid and can hold no more.
//If it is a string, it is passed to
//my_action().
void set_full_action( mixed x ){ full_action = x; }


varargs void full_action( object ob )
{

  if (functionp(full_action))
  {
    evaluate ( full_action, ob );
    return;
  }

  if (!this_body() )
    return;
  this_body() -> my_action ( full_action, ob -> short(), this_object() );

}

varargs object fluid_already ( object fluid, string other_id )
{
  object fluid_already;

  if (!other_id)
    other_id = fluid -> query_primary_id();
  fluid_already = present( other_id, this_object() );

  if ( !fluid_already ) 
    return 0;
  if ( !fluid_already -> id( "fluid" ) )
    return 0;

  return fluid_already;
}

//:FUNCTION move_fluid
//Moves fluid to fill the continer - if successful
//it fills the container
void move_fluid( object ob )
{
  mixed res =  ob -> move ( this_object() );
  if(res == MOVE_OK)
    ob -> set_fluid_level( query_max_capacity() );
}

//:FUNCTION part_move_fluid
//Moves fluid to fill the continer - if successful the quantity
//transferred is limited to the original quantity
void part_move_fluid( object ob )
{
  mixed res =  ob -> move ( this_object() );
  if(res == MOVE_OK)
    ob -> set_fluid_level( this_object()->query_capacity() );
}

void drink_from_it( object fluid ) 
{
  object ob;
    
  ob = first_inventory();
  if (!ob)
  {
    write ("Pity there's nothing in it.\n");
    return;
  }
  ob -> drink_it();
}

//:FUNCTION fill_with
//Attempts to completely fill the container
//(eg fill from source, such as tap)
int fill_with( object fluid )
{
  if (!can_hold_fluid( fluid ) ) {
    return leak( fluid );
  }
  if (!will_fit( fluid ) )
  { 
    full_action( fluid ); 
    return 0; 
  }
  if ( fluid_already( fluid ) ) {
    return (fluid_already(fluid)) -> add_fluid( fluid );
  }
  if ( fluid_already( fluid, "fluid" ) ) {
    return fluid_already( fluid, "fluid" ) -> do_mix( fluid );
  }
  if (!fluid -> fill_action( this_object() ))
    return 0; //Give the fluid a chance to have a say.
  move_fluid( fluid );
  return 1;
}

//:FUNCTION part_fill_with
//Attempts to fill the container, but limited to amount of fluid
//(eg fill one container from another, by "pour")
int part_fill_with( object fluid )
{
  if (!can_hold_fluid( fluid ) )
    return leak( fluid );
  if (!will_fit( fluid ) )
  { 
    full_action( fluid ); 
    return 0; 
  }
  if ( fluid_already( fluid ) )
    return fluid_already(fluid) -> add_fluid( fluid );
  if ( fluid_already( fluid, "fluid" ) )
    return fluid_already( fluid, "fluid" ) -> do_mix( fluid );
  if (!fluid -> fill_action( this_object() ))
    return 0; //Give the fluid a chance to have a say.

  part_move_fluid( fluid );
  return 1;
}

varargs mixed pour( string how, object where, object from )
{

  object fluid;

  fluid = first_inventory();
  if ( !fluid ) 
    return ( this_object() -> the_short() + " is empty.");


  fluid -> pour (how, where, this_object());

  return 1;
}

string get_extra_long()
{
  string full;
  object ob = first_inventory();
  if (!ob)
    full = "It is empty";
  else
#ifdef USE_SIZE
    switch ((100 * ob->query_size())/ this_object()->query_max_capacity())
#endif
#ifdef USE_MASS
    switch ((100 * ob->query_mass())/ this_object()->query_max_capacity())
#endif
    {
    case 0: 
      full = "It is empty.";
      break;
    case 1..20:
      full = "It is almost empty.";
      break;
    case 21..40:
      full = "It is less than half full.";
      break;
    case 41..60:
      full = "It is about half full.";
      break;
    case 61..80:
      full = "It is more than half full.";
      break;
    case 81..99:
      full = "It is almost full.";
      break;
    case 100: 
      full = "It is full to the brim.";
      break;
    }
  return full + "\n";
}

mixed have_flask()
{
  if (!(environment() == this_body() ) ) 
    return "You do not have "+the_short()+".";
  return 1;
}

mixed indirect_pour_obj_wrd_obj()
{
  return 1;
//  return have_flask() ;
}

mixed direct_pour_obj_wrd_obj(){ return have_flask() ; }

mixed direct_pour_obj()
{ 
// For 'empty' synonym.
  return have_flask();
}

mixed direct_fill_obj( ){ return have_flask(); }

mixed direct_fill_obj_with_obj(){ return have_flask(); }

mixed direct_fill_obj_with_str(){ return have_flask(); }

mixed direct_fill_obj_from_obj(){ return have_flask(); }

mixed direct_drink_from_obj(){ return have_flask(); }

mixed indirect_drink_obj_from_obj(){ return have_flask(); }
