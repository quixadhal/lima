/* Do not remove the headers from this file! see /USAGE for more info. */
// The drink source module.
//  Coded by Uae of Zork.  2/20/98.

//:MODULE
//This module picks up where m_fluid_source
//leaves off.  It allows drinking from a source.

mixed drink_action = "$N $vdrink some $o from $o1.";
mixed fluid_disturb;

void remove_id( string array id... );
object is_source_of();
mixed get_fluid_disturb_action();
string query_primary_id();
string *query_id();

//:FUNCTION set_drink_action
//void set_drink_action( mixed x )
//x is a simple_action string or
//a function that is evaluated whenever
//this source of fluid is drunk from.
//x can contain $o to represent the fluid.
//$o1 can represent the drink source.
void set_drink_action( mixed x ){
  if (!x) remove_id( "drink_source" );
  drink_action = x;
}

int drink_from_it( mixed fluid ){

   mixed taste_action;
   object fluid_ob;
   int cntinue =1;

   fluid_ob = new( is_source_of() );

   if (functionp( drink_action )) cntinue = evaluate( drink_action );
   else this_body()-> simple_action
 	( drink_action, fluid_ob -> query_primary_name(), this_object() -> the_short() );

   if (!cntinue) return 0;

   fluid_disturb = get_fluid_disturb_action();

   if (fluid_disturb)
	{
        if (functionp( fluid_disturb )) cntinue = evaluate( fluid_disturb );
	else
	this_body() -> my_action( fluid_disturb, fluid, this_object() );
	};

   if (!cntinue) return 0;

   taste_action = fluid_ob -> get_taste_action();

   if (taste_action)
	{
	if (functionp( taste_action )) return evaluate( taste_action) ;
	this_body()-> my_action( taste_action, fluid_ob );
	};

   return 1;
}

int is_drink_source(){
  return 1;
}

mixed direct_drink_obj(){
return "#You cannot drink an entire " + query_primary_id();
}

mixed direct_drink_str_from_obj( string fluid ){
if ( !member_array( fluid+"_source", query_id() )) return "You cannot drink "+fluid+" from that.\n";
return 1;
}

mixed indirect_drink_obj_from_obj()
{
return 1;
}

mixed direct_drink_from_obj()
{
return 1;
}
