/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;

void setup()
{
  set_adj( "camera", "polaroid" );
  set_id( "button" );
  set_long( "It's a small black button." );
  set_getmsg( "#That would surely break the camera.");
#ifdef USE_SIZE
  set_size(SMALL);
#endif
#ifdef USE_MASS
  set_mass(SMALL);
#endif
}

void do_press( string s )
{
  environment( this_object())->take_picture();
}
