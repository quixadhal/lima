/* Do not remove the headers from this file! see /USAGE for more info. */

// Modified by Uae.  May 1997.

//:MODULE m_fluid_container
//Fluids can never be 'gotten'.
//Most containers leak.  Inherit this module into
//your container so that it will fill and pour.

int query_max_capacity();
string the_short();

mixed leak_action = "$O leaks out of $o1.";
mixed full_action = "The $o1 is full of $o.";
mixed fluid_only = 1;

void look_me();

//:FUNCTION can_hold_fluid
//int can_hold_fluid( mixed fluid_ob )
//When fluid is passed into the fluid
//container, this function is called with
//the fluid object as the argument.  returns
//1 by default, but this function can be
//overridden if this container is limited 
//in the types of fluids it can hold.
varargs int can_hold_fluid( mixed ob ){
  return 1; 
}

//### Incompletelly implemented.
void set_fluid_only( mixed x )
{
  fluid_only = x;
}

mixed query_fluid_only()
{
  return fluid_only;
}

//:FUNCTION set_leak_action
//set_leak_action(x)
// 'x' is evaluated if this
//container cannot hold the fluid
//that is being put into it.
void set_leak_action( mixed x ){
  leak_action = x;
}

void leak ( object ob ){
  if (functionp(leak_action))
	{
	evaluate ( leak_action, ob );
	return;
	}

  if (!this_body() ) ob -> remove();
  this_body() -> simple_action ( leak_action, ob, this_object() );
  ob -> move( environment( this_body() )  );
}

int will_fit( object fluid ){
	int fluid_size, room_left, load;

	if ( first_inventory() ) load = first_inventory() -> get_size();
	fluid_size = fluid ->get_size();
	room_left = query_max_capacity() - load;
	//write ("Fluid size:  " + fluid_size + "   Room left:   " + room_left + ".\n");
	return (fluid_size <= room_left );
}

//:FUNCTION set_full_action
//set_full_action(x)
//'x' is evaluated when this container
//is full of fluid and can hold no more.
//If it is a string, it is passed to
//my_action().
void set_full_action( mixed x ){
  full_action = x;
}


varargs void full_action( object ob ){

  if (functionp(full_action))
	{
	evaluate ( full_action, ob );
	return;
	}

  if (!this_body() ) return;
  this_body() -> my_action ( full_action, ob -> short(), this_object() );

}

varargs object fluid_already ( object fluid, string other_id ){

  object fluid_already;

  if (!other_id) other_id = fluid -> query_primary_id();
  fluid_already = present( other_id, this_object() );

  if ( !fluid_already ) return 0;
  if ( !fluid_already -> id( "fluid" ) ) return 0;

  return fluid_already;
}


void move_fluid( object ob ){
  ob -> move ( this_object() );
  ob -> set_fluid_level( query_max_capacity() );
}

void drink_from_it( object fluid ) {
    object ob;
    
    ob = first_inventory();

    ob -> drink_it();
}

int fill_with( object fluid ){
  if (!can_hold_fluid( fluid ) ) return leak( fluid );
  if (!will_fit( fluid ) ){ full_action( fluid ); return 0; }
  if ( fluid_already( fluid ) ) return fluid -> add_fluid( fluid_already( fluid ));
  if ( fluid_already( fluid, "fluid" ) ) return fluid_already( fluid, "fluid" ) -> do_mix( fluid );
  if (!fluid -> fill_action( this_object() )) return 0; //Give the fluid a chance to have a say.
  move_fluid( fluid );
  return 1;
}

varargs mixed pour( string how, object where, object from ){

  object fluid;

  fluid = first_inventory();
  if ( !fluid ) return ( this_object() -> the_short() + " is empty.");

  fluid -> pour (how, where, this_object());

  return 1;
}

mixed have_flask(){
if (!(environment() == this_body() ) ) return "You do not have "+the_short()+".";
return 1;
}

mixed indirect_pour_obj_wrd_obj(){
return have_flask() ;
}

mixed direct_pour_obj(){ // For 'empty' synonym.
return have_flask();
}

mixed direct_fill_obj( ){
return have_flask();
}

mixed direct_fill_obj_with_obj(){
return have_flask();
}

mixed direct_fill_obj_with_str(){
return have_flask();
}

mixed direct_fill_obj_from_obj(){
return have_flask();
}

mixed direct_drink_from_obj(){
return have_flask();
}

mixed indirect_drink_obj_from_obj(){
return have_flask();
}

