/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
// A complex hidden exit object.


inherit COMPLEX_EXIT_OBJ;

//:FUNCTION set_destination
// Set where you go when you enter the portal.  Valid arguments are a function
//pointer,  or a filename
void set_destination(mixed dest)
{
  /* This is to make the pathname relative to the room that the door is in 
   * rather than the door itself */
  if(stringp(dest)) {
    dest=(:absolute_path($(dest),environment()):);
  }

  add_method("go",dest);
}

string get_destination()
{
  return query_method_destination("go");
}

varargs void mudlib_setup(string destination)
{
    ::mudlib_setup();
    set_attached();
    if ( destination ) add_method( "go", destination );
}
