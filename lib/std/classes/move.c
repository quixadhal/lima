/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * This class contains the data relavent to moving objects from one place to 
 * another.
 * Written by Tigran 3/23/99
 */

/* 
 * This really needs to be included rathar than inherited as would be ideal
 * otherwise NON_ROOM and such will not compile due to multiple inheritance of
 * this file
 */

class move_data
{
  mixed destination;            /* This can be either a string file_name() or 
				 * an object */
  string relation;              /* A string indicating the relation which the 
				 * object being moved is going to.  For example
				 * "in", "on", "over" */
  mixed exit_dir;               /* A string which indicates the direction
				 * the exit taken */
  mixed exit_messages;
  mixed enter_messages;
  mixed checks;                 /* Checks whether or not the move is going 
				 * to be possible */
  object who;                   /* Who is actually moving */
  string description;           /* The description of the exit, for simple 
				 * exits */
  object source;                /* The source object.  Used for complex exits
				 * and messaging */
object through;                 /* The object being moved enters at the
                                 * destination through this object.
                                 * Used for complex exits and messaging. */
}
