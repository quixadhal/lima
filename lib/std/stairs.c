/* Do not remove the headers from this file! see /USAGE for more info. */

inherit COMPLEX_EXIT_OBJ;

mixed direct_get_obj( object ob ) {
    if( query_plural())
        return "#Try climbing them instead.";
    else
        return "#Try climbing it instead.";
}

varargs protected void setup_messages(string name, mixed up_dest, mixed down_dest)
{
  string dirs;
  if(up_dest)
    add_method("ascend",up_dest);
  if(down_dest)
    add_method("descend",down_dest);
  if (up_dest)
    {
      if (down_dest)
	dirs = "up and down";
      else
	dirs = "upwards";
    }
  else
    {
      if( down_dest )
	dirs = "downwards";
      else
	dirs = "nowhere";
    }
  if (query_plural())
    add_id_no_plural(name);
  else
    add_id( name );
  set_long(sprintf("The %s %s %s",
		   name,
		   query_plural()?"lead":"leads",
		   dirs) );
  set_in_room_desc(sprintf("There %s %s here, leading %s.",
			   query_plural()?"are ":"is a ",
			   name,
			   dirs) );
}


// Separated from create() so that objects can overload this separately
void more_create(mixed up_dest, mixed down_dest) {
    set_plural(1);
    setup_messages("stairs", up_dest, down_dest);
}


void mudlib_setup( mixed up_dest, mixed down_dest)
{
    ::mudlib_setup();
    more_create( up_dest, down_dest );
    set_base((:environment():) );
    set_attached( 1 );
}
