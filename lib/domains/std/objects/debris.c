/* Do not remove the headers from this file! see /USAGE for more info. */
#include <move.h>

inherit OBJ;
inherit M_GETTABLE;

void setup()
{
  set_id("rock", "rocks", "debris", "rubble");
  set_attached(1);
  set_long("The rocks aren't very interesting.");
#ifdef USE_SIZE
  set_size(TOO_LARGE);
#endif
#ifdef USE_MASS
  set_mass(TOO_LARGE);
#endif
}

mixed get()
{
  object env = environment();	
  mixed  r;

  if((r = ::get()) == MOVE_OK)
  {
    this_body()->simple_action ("$N $vpick up a rock.");
    new(__DIR__+  "rock")->move(this_body());
    return MOVE_NO_ERROR;
  }
  return r;
}
