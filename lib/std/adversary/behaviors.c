/* Do not remove the headers from this file! see /USAGE for more info. */

void dispatch_opponent();
void do_game_command(string str);

void do_move_away();

//:FUNCTION flee
// Try to run away. Does nothing by default. Overload this function to
// modify the behavior of your monster when it panics.
void flee()
{
//  do_move_away();
}

//:FUNCTION surrender
// Try to surrender. Does nothing by default. Overload this function to
// modify the behavior of your monster when it panicss.
void surrender()
{
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

//:FUNCTION do_move_away
//Moves through a random exit. Probable implementation of "flee"
void do_move_away()
{
  string array directions;
  string chosen_dir;
  string file;
  object dest;
  
  if(environment(this_object()))
    directions=environment(this_object())->query_exit_directions();

/* Stop if there are no directions (this takes care of blue prints*/
  if(!sizeof(directions))
    return;

  chosen_dir=choice(directions);
  file=environment(this_object())->query_exit_destination(chosen_dir);

/* If the file is null or a null string then give up */
  if(!file||file=="")
    return;

/* If the destination is not loaded, do so */
  dest=load_object(file);

/* If still no destination it won't load, try again next time */
  if(!dest)
    return;

  do_game_command(sprintf("go %s",chosen_dir));
}
