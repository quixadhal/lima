/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE portal
// A simple one way portal

inherit COMPLEX_EXIT_OBJ;
inherit M_GETTABLE;

string look_in_desc;


//:FUNCTION set_destination
// Set where you go when you enter the portal.  Valid arguments are a function
//pointer or a filename
void set_destination(function f)
{
  add_method("enter",f);
}

string get_destination()
{
  return query_method_destination("enter");
}

int direct_look_str_obj()
{
    return 1;
}

//:FUNCTION set_look_in_desc
// Set what you see when you look in the portal.
void set_look_in_desc(string s)
{
    look_in_desc = s;
}

string get_look_in_desc()
{
    return evaluate(look_in_desc);
}

string look_in(string prep)
{
    return evaluate(look_in_desc);
}


mapping lpscript_attributes() {
    return ([
      "look_in_desc" : ({ LPSCRIPT_STRING, "setup", "set_look_in_desc" })
    ]);
}


varargs void mudlib_setup(string destination)
{
    ::mudlib_setup();
    set_id( "portal" );
    set_attached();
    set_gettable( "#It's a portal not a portable.");
    if ( destination ) add_method( "enter", destination );
}
