/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;

// This checks whether to allow an object to enter the car wash
// It prohibits livings from entering, and returns an appropriate message
// telling them to get in a car, or remain in the car
int receive_object( object target, string relation )
{
  object env = environment( target );
  if( target->is_living())
  {
    if(env->is_vehicle() && member_array( "car", env->query_id()) > -1)
      tell(target, "It'd be best to stay in the car!\n");
    else
      tell(target, "It'd be best to be in a car to enter the car wash!\n");
    return 0;
  }
  return ::receive_object( target, relation );
}


void setup()
{
  set_brief("Car wash");
  set_long("The car wash is one of the touchless models. ");
  set_exits( ([ "west" : "Example_Room1" ]));
//  set_enter_msg("west", "As $n $venter the car wash, jets of water begin washing the under carriage of $p car. \n");
}
