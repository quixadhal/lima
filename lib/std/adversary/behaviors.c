/* Do not remove the headers from this file! see /USAGE for more info. */

void dispatch_opponent();

/* Override flee() and surrender() to define or modify the behaviour
 * of your monster.  Return 1 of the monster shouldn't take a swing.
 * Return 2 if the monster should stop attacking. (useful for
 * surrender() 
 */
//:FUNCTION flee
//Try to run away.
void flee() {
}

//:FUNCTION flee
//Try to run surrender
void surrender() {
}

//:FUNCTION panic
//Do something intelligent when we are about to die
/* Override this function to have your monster do something else when it's
 * hp's get low. 
 */
void panic()
{
   if(random(10) < 3)
      flee();
   else surrender();
}

//:FUNCTION target_is_asleep
//Called with the person we are attacking is asleep or unconscious.  Default
//behavior is to finish them off.

/* Override this function if you want your monster to do something other
 * than killing unconscious victims.  Rob them, etc ...
 */
void target_is_asleep()
{
   dispatch_opponent();
}
