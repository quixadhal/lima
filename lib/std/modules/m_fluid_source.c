/* Do not remove the headers from this file! see /USAGE for more info. */
//:MODULE
//Inherit this module into anything that
//is a source for fluid.  Rivers, fountains,
//bee-hives, volcanoes, corpses, etc.
//Fill and drink look for a fluid source.
//To be able to drink directly from the
//source, also inherit m_drink_source.
//in the environment.
#define PATH_TO_OBJECTS __DIR__

void add_id( string x );
string array query_id();
string query_primary_id();

string source_of;
mixed fluid_disturb = "The $o1 ripples gently as you disturb it.";
mixed fill_action = "$N $vdip $p $o into $o1 and $vwithdraw some $o2.";

//:FUNCTION set_source_of
//set_source_of( string x )
//x must be a path name to a fluid object.
//new(x) is what is moved into a container
//when the container is filled from here.
void set_source_of( string what  ){
  object ob;
  string *ids;

  ob = new( what );
  ids = ob -> query_id();

  foreach( string id in ids ) add_id( id + "_source" );
  add_id ( "fluid_source" );
  add_id ( "fill_source" );
  add_id ( "drink_source" );

  source_of = what;
  
}

string is_source_of(){
return source_of;
}//this is the function that is queried
//when the fluid object is cloned.
//A clever coder in need could have several fluids
//from the same source by overriding this function.
//Note that all of the ids would need to have been a part of
//the id of the object is set_source_of.


//:FUNCTION set_fluid_disturb_action
//set_fluid_disturb_action( x )
//'x' is evaluated whenever the fluid is
//disturbed.  As a string, x is passed
//to my_action.  If you don't want any
//disturb_action, set x to 0.
void set_fluid_disturb_action( mixed x ){
  fluid_disturb = x;
}

mixed get_fluid_disturb_action(){
  return fluid_disturb;
}

//:FUNCTION set_fill_action
//set_fluid_disturb_action( string x )
//'x' is passed to simple_action. 
//$o in the string represents the fluid container.
//$o1 represents the fluid source.
//$o2 represents the fluid itself.
void set_fill_action( string x ){
  fill_action = x;
}

mixed fill_up(object bottle){
	object fluid;

	fluid = new(is_source_of() );

	if (bottle -> fill_with( fluid ))

	{
	this_body() -> simple_action
	( fill_action, bottle -> short(), this_object() -> the_short(), fluid);
	
	if (fluid_disturb) this_body() -> my_action( fluid_disturb, fluid, this_object() );

	return 1;
	}

	else return 0;
}

// Verb stuff:


mixed direct_drink_obj(){
return "#You cannot drink an entire " + query_primary_id();
}

mixed indirect_drink_str_from_obj( string fluid, object ob ){
if ( !member_array( fluid+"_source", query_id() )) return "You cannot drink "+fluid+" from that.\n";
return 1;
}

mixed direct_drink_str_from_obj( string fluid, object ob ){
if ( !member_array( fluid+"_source", query_id() )) return "You cannot drink "+fluid+" from that.\n";
return 1;
}

int indirect_fill_obj_from_obj(){
return 1;
}

mixed indirect_drink_obj_from_obj()
{
return 1;
}
