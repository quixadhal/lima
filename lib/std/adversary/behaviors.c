/* Do not remove the headers from this file! see /USAGE for more info. */

void dispatch_opponent();

//:FUNCTION flee
// Try to run away. Does nothing by default. Overload this function to
// modify the behavior of your monster when it panics.
void flee() {
}

//:FUNCTION surrender
// Try to surrender. Does nothing by default. Overload this function to
// modify the behavior of your monster when it panicss.
void surrender() {
}

//:FUNCTION panic
// Do something intelligent when we are about to die. Overload this
// to have your monster do something else when it's hp's get low.
// The default behavior is to randomly flee or surrender.
void panic()
{
   if(random(10) < 3)
      flee();
   else surrender();
}

//:FUNCTION target_is_asleep
// Called with the person we are attacking is asleep or unconscious.
// Default behavior is to finish them off. Overload this function if you
// want your monster to do something other than killing its victims.
// Rob them, etc..
void target_is_asleep()
{
   dispatch_opponent();
}
