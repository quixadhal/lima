/* Do not remove the headers from this file! see /USAGE for more info. */

inherit COMPLEX_EXIT_OBJ;

void award_points(object ob,string direction);

function award=(:award_points :);

void setup()
{
  set_attached(1);
  set_id("ocean","water","surf","waves");
  set_long("There is nothing special about the ocean.");
  /* set_base()
   * Sets the base directory from which relative paths are figured.  For this
   * object, it should be the environment of this object, so pass that 
   * function pointer to it */
  set_base((:environment():) );
  /* add_method()
   * The first argument is the method to be used.  In this case we want to 
   * 'wade in the ocean'.  
   * The second argument is the destination that the body is going to be moved 
   * to when the method is invoked.
   * The third is the check to see if the body can actually use the method,
   * The fourth argument is an array of possible exit messages
   * The fifth argument is an array of possible enter messages */
  add_method("wade in",
	     "Outside_Cave",
	     1,
	     ({ "$N $vwade into the ocean.  Though the undercurrent is "
		  "strong, $n $vmanage to keep $p balance.  $N $vwander "
		  "towards the east, where there is no beach, but there are "
		  "interesting mountains...",
		"$N $vwade around in the ocean for a while, "
		  "eventually wandering out of sight to the east."}),
	     ({ "$N $vwade into the ocean.  Though the undercurrent is "
		  "strong, $n $vmanage to keep $p balance.",
		"$N $vwander in from the west",
		"$N $vwade over to you."}) );
}

void award_points(string direction)
{
tell_user("loriel","dir : " + direction);
  if(direction==query_direction())
    QUEST_D->grant_points(this_body(),"pirate:foundCave");
}

void on_clone(mixed args...)
{
  ::on_clone(args...);
  /* Add the hook to award quest points to the environment of this object */
  environment()->add_hook("player_left",award);
}
