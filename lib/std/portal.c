/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE portal
// A simple one way portal

inherit EXIT_OBJ;
inherit M_GETTABLE;

string look_in_desc;


//:FUNCTION set_destination
// Set where you go when you enter the portal
void set_destination(string s)
{
    set_default_destination(s);
}

string get_destination()
{
    return query_default_destination();
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
    if ( destination ) set_default_destination( destination );
    set_go_method("enter");
//    set_default_exit_msg( (: prep_default_msg :) );
//    set_default_enter_msg( (: prep_default_msg :) );
}
