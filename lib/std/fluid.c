/* Do not remove the headers from this file! see /USAGE for more info. */

//Fluid objects act differently from the typical
//mud object.  Properly, fluid should not be able to
//be handled at all except using a container.
//Coded by Uae of Zork.  9/20/97

inherit OBJ;
inherit M_GETTABLE;
inherit M_MESSAGES;

int evap_amount = 6 ;
int evap_time = 20;

void remove_puddle_func();
void evaporate_puddle_func();

string puddle_id = "puddle"; // This could be changed, for some things, to pile...
string puddle_name;

mixed pour_action = "$N $vpour $o $o1 $o2.";
mixed taste_action = "It tastes rather bland.";

mixed evaporate_action = 0;
function evaporate_puddle = (: evaporate_puddle_func :);

string short();
int ob_state();

//:FUNCTION set_puddle_name
//void set_puddle_name( string x )
//'x' is set as the proper name
//of this object when it is outside
//of a container. Defaults to 
//'the <puddle_id> of <primary_name>'
void set_puddle_name( string x ){
  puddle_name = x;
}

//:FUNCTION set_puddle_id
//void set_puddle_id( x )
//adds id 'x' when the fluid is
//outside of a container.
void set_puddle_id( string x ){
  puddle_id = x;
}


string query_puddle_name(){
  if (!puddle_name) return "the " + puddle_id + " of " + query_primary_name();
  return puddle_name;
}

//:FUNCTION set_evaporate_action
//set_evaporate_action( mixed x )
//'x' is optional.
//Calling this function with no arguments
//will cause the fluid to evaporate when 
//outside of a container.  
//x can be a string that is output to the
//environment as the fluid evaporate_puddle
//function is called.
//x as a function will be evaluated when
//the fluid is moved outside of a container.
//Do not call this function if the fluid
//is not to evaporate.
varargs void set_evaporate_action( mixed x ){
   if (!x) evaporate_action = "The $o dwindles in size.";
   else evaporate_action = x;
}

//:FUNCTION set_pour_action
//void set_pour_action( mixed x )
//'x' is a simple action string or
//a function that is evaluated whenever
//this fluid is poured.  x may contain
//$o to represent the fluid itself.
void set_pour_action( mixed x ){
  pour_action = x;
}

void create(){
  ::create();
  add_id( "fluid", "liquid", "watery" );// watery can be used to exclude fluids that are not viscous.
  set_quantity( 1 );
}

varargs void set_fluid_level( int x, string y ){
  set_size( x );
  if ( y ) set_quantity( y );
  else set_quantity( x );
}

varargs int add_fluid( object fluid){
  if (!fluid) return 0;
  set_fluid_level( get_size() + (fluid -> get_size()) );
  fluid -> remove();
  return 1;
}

int do_mix( object fluid ){ //This is meant to be overridden.
	add_fluid( fluid );
	return 1;
}

varargs object fluid_already( string x, object env ){

  object ob;

  if (!env) env = environment( this_body() );

  if (!x) x = query_primary_id();
  ob = present( x, env );

  if (!ob) return 0;

  if ( !ob -> id("fluid") ) return 0;

  return ob;
}

varargs void pour( string how, mixed ob2, mixed from ){

	string where;

	if ( how == "from" || how == "out" ) ob2 = environment( this_body() );

	if ( ob2 == environment( this_body() ) ) 
		{
		how = "onto";
		where = "the ground";
		}
	else where = ob2 -> the_short();


	if ( ob2 == environment( this_body() ) )
		{
		if (functionp( pour_action ) ) evaluate( pour_action, how, ob2 ); else
		this_body() -> simple_action ( pour_action, this_object(), how, where );
		if ( add_fluid( fluid_already() )) return;
		if (evaporate_action)
			{
			remove_call_out( 0 );
			if (functionp(evaporate_action)) evaluate( evaporate_action);
			else call_out( evaporate_puddle , evap_amount );
			};
		move( ob2 );
		remove_adj( query_primary_adj() );// This avoids strange bugs.
		set_proper_name( query_puddle_name() );
		add_id( puddle_id );
		}


	else if ( ob2 -> fill_with( this_object() ))
		{
		if (functionp( pour_action ) ) evaluate( pour_action ); else
		this_body() -> simple_action ( pour_action, this_object(), how, where );
		};



}
	

int will_fit( object bottle ){
	int fluid_size;
	int room_left;
	fluid_size = get_size();
	room_left = 
	bottle -> query_max_capacity() 
	   - bottle -> query_capacity() ;

	return (fluid_size <= room_left);
}

mixed move_fluid ( object bottle ){

	if (!will_fit( bottle )) return 0;

	move( bottle );
	return 1;
}

void remove_puddle(){
	remove_call_out( 0 );
	simple_action("The $o dwindles to nothing.", this_object() );
	remove();
}

void evaporate_puddle_func(){

   set_size( get_size() -1 );

   if( get_size() <= 0 ) {remove_puddle(); return;};

    this_object() -> simple_action
	( evaporate_action, this_object() );

   call_out( evaporate_puddle, evap_time );
}

mixed drop(){
  mixed msg;

  msg = ::drop();

  if (msg)
	{
	add_id( puddle_id );
  	add_fluid( fluid_already() );
	};

  return msg;
}

mixed get(){
  mixed msg;

  msg = ::get();

  if (msg)
	{
	remove_id( puddle_id );
  	add_fluid( fluid_already( 0, this_body() ) );
	};

  return msg;
}

//By default, fluids are not drinkable.
void drink_it(){

  this_body() -> my_action( "It is not a good idea to drink $o.", short() );
}

//:FUNCTION fill_action
//fill_action(container)
//This function is called by the container
//in the fluid just before it fills the container.
//The container passes itself as an argument.
//This function must return a 1 for the filling
//to continue.  This gives the fluid itself
//an opportunity to halt the filling action.
int fill_action( object vessel ){ return 1; }

int have_fluid(){
  if (environment( environment( this_object() ) ) == this_body() )
  return 1;
  return 0;
}

int direct_pour_obj(){
  return have_fluid();
}

int direct_pour_obj_out(){
return have_fluid();
}

int direct_pour_obj_wrd_obj(){
return have_fluid();
}

int indirect_fill_obj_with_obj() {
return 1;
}

mixed direct_drink_obj(){ return 1;}

mixed direct_drink_obj_from_obj(){
   return 1;
}
