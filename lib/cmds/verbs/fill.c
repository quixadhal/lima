/* Do not remove the headers from this file! see /USAGE for more info. */

// Modified by Uae.  May 22nd. 1997.

// Modified to deal with more fluids than water:
// In the fluid_object, add "fluid" to the id.

// Modified to fill fluid containers from renewable sources:
// In the source of the fluid, add "<fluid>_source" to the id.
//  "fill <ob> with <fluid>" will call fill_up( ob ) in the fluid source.
// Use that function to move the relevant fluid to the container.

// May 23rd.  Added functionality for fill_obj_from_obj.

#include <mudlib.h>
#include <setbit.h>

inherit NVERB_OB;

int do_fill_it(object bottle, object fluid)
{
    if (!fluid ) {
	fluid = present("fill_source", environment( this_body () ));
	if (fluid) { fluid -> fill_up( bottle) ;
	   	return 1;};

	fluid = present("fluid", environment(this_body()))
	            || present("fluid", this_body()); // This should never be!
	
	if (!fluid) {
	    write("With what?\n");
	    return 1;}
	};
    write("(with " + fluid -> short() +")\n");
    bottle ->fill_with( fluid ); //fluid_obj: check to see if it is in presence of source.
    return 1;
}

// This function looks for an object with id ("<string>_source")
// and calls the fill_up( ob ) function in that object.
// To have a source of any <fluid>, add an id of <fluid>_source.
//  Use the function fill_up( ob ) to move the fluid into ob.

int source( object bottle, string fluid )
{
   object source;

   source = present( fluid +"_source", environment( this_body()) );
   if (!source ) {
	write("You cannot fill "
      	+ bottle -> the_short() + 
	" with " + fluid + ". \n");
	return 1; };
   return source -> fill_up( bottle );
} 

void do_fill_obj(object ob)
{
    do_fill_it(ob, 0);
}

void do_fill_obj_with_obj(object ob1, object ob2) {
    do_fill_it(ob1, ob2);
}

void do_fill_obj_with_str(object ob1, string str ){
  source( ob1, str );
}

void do_fill_obj_from_obj( object ob1, object ob2){
  ob2 -> fill_up( ob1 );
}

void create()
{
   add_rules ( ({ "OBJ",  "OBJ with OBJ" , "OBJ with STR" , "OBJ from OBJ" }) );
}



