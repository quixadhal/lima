/* Do not remove the headers from this file! see /USAGE for more info. */
/*                                                             */
/*   This is an example room for the lima mudlib          */
/*                                                             */
/*   Created by zifnab@lima bean 7/24/95                         */
/*                                                             */
/****************************************************************/

inherit ROOM;


/*
 * This is a special case function.  That allows you to check for 
 * a condition before deciding to let the person enter your room.
 * there are multiple ways to do this.  See help exits for more 
 * information.
 */
mixed can_go_east()
{
  object env = environment( this_body());
  if( env->is_vehicle() && member_array( "car", env->query_id()) != -1)
    {
      return 1;
    }
  return "Perhaps it would be better if you were in a car.\n";
}

/*
 * Setup() is where you define what makes your room differant from 
 * the other rooms on the mud.  Setup() in the Lima mudlib is used instead
 * of create() in the rooms/objects.
 */
void setup(){
  
  /*
   * set_area() allows to give a "name" to your area.  This currently
   * is used in the lib to control what rooms a wandering monster can 
   * "wander" into.
   */
  set_area("wiz_area");
  
  /*
   * set_brief() this is the "short" description of the room. This will
   * appear on the top line if you do a look in the room.  Also if the player
   * has brief on, this is what they will see. 
   */  
  set_brief("Example Room");
  
  /*
   * set_long() is the long description of the room.  This should be as
   * descriptive as possible.  But try not to go over 5-6 lines unless its
   * necessary, people dont want to read long descriptions that are not 
   * interesting.
  */  
  set_long("This is an example room for the lima mudlib.  "
	   "It is fairly non-descript however there are a few objects laying "
	   "about that may warrant looking at. Propped up in the southwest "
	   "corner is an old discarded suit of armor.  On the west side of "
	   "the room is an arched doorway leading bask to the wizard lounge. "
	   "For some odd reason there is a car wash to the east!");
  
  /*  
   * set_exits() is where you specify the exits that your room will have.
   * if the room that the exit leads to is in the same directory, all you 
   * need to do is specify the room name. or you can use __DIR__ (the driver
   * recognizes this as the current directory), and it makes things a bit
   *  clearer.  for more information on what you can do see help exits.
   */  
  set_exits( ([
	       "west" : "Wizroom",
	       "south" : "Relations_And_Toys",
	       "north" : "/domains/std/rooms/V_Plains/4/7",
	       "east" : "Car_Wash",
	       ]) );
  
  /*  
   * set_exit_msg()  allows you to give your room a unique message when leaving
   * the room, rather than seeing foo leaves..  The players inthe room, and 
   * the player moving will see your message.  set_enter_msg would be used for
   * entrance messages.
   */


  /*
   * add_item()  Allows for items that are used for scenery only.  These items 
   * are not meant to be used for anything real.  for example a player cannot
   * get these items, basically they are for scenery and atmosphere only.  If
   * want the player to do anythign "real" use set_objects( ) and make a "real"
   * object.
   */
  add_item( "suit" , "armor" , "suit of armor" , 
	    "The armor is very old and well worn.  While examining the "
	    "armor you imagine you hear the sounds of combat coming from "
	    "behind you.");
  
  /*
   * set_objects() is where put your real objects.  The first part if the 
   * actual object.  If this is in the directory again you only need to
   * specify the objects file name, not the entire path.  __DIR__ is valid 
   * here also. The number following the : are the number of this particular 
   * object to clone and move to the room.  when reset() is called in this
   * room, if the safe no longer exists in this room it will be cloned and 
   * moved into the room.
   */
    set_objects ( ([
      ]));
}





