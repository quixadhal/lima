/* Do not remove the headers from this file! see /USAGE for more info. */


//:PLAYERCOMMAND
//$$ see: finger
//USAGE plan
//
//Takes you to an editor where you can enter or change your plan,
//which everybody can see when they "finger" you.

#include <edit.h>

inherit CMD;

private nomask void set_plan(string * plan)
{
  if( !sizeof( plan ))
  {
    write( "Your plan's unchanged, man.\n");
    return;
  }
  write("Your plan is set, man.\n");
  this_body()->set_plan(implode(plan,"\n"));
}

private void main()
{
#ifdef EVERYONE_HAS_A_PLAN
  out("Enter your plan, Stan.\n");
  new(EDIT_OB, EDIT_TEXT, 0, (: set_plan :));
#else
  out("Sorry, plans are not available.\n");
#endif
}
