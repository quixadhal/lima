/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** msg_recipient.c -- handle message passing for objects
**
** DOWNWARD: into the contents of the object
** UPWARD:   up to this object's container
**
** 960603, Deathblade: added this header re: directions
*/

//:FUNCTION environment_can_here
//Returns 1 if messages should propagate upwards to our environment.
int environment_can_hear()
{
  object env = environment();

  return env && !env->cant_hear_contents();
}

//:FUNCTION receive_inside_msg
//Handle a message received from inside us (which should never happen, but...)
//Inside messages propogate upward and downward, but we have no downward
varargs void
receive_inside_msg(string msg, object array exclude, int message_type, 
			mixed other)
{
  object env;

  receive(msg);

  if(environment_can_hear() && (env = environment()) && 
     (!arrayp(exclude) || member_array(env,exclude) == -1))
    {
      env->receive_inside_msg(msg, arrayp(exclude) ? exclude + 
			      ({this_object()}) : ({this_object()}), 
			      message_type, other);
    }

}

//:FUNCTION receive_outside_msg
//Receive a message from outside of us.
//Outside messages propogate downward, so just receive
varargs void
receive_outside_msg(string msg, object array exclude, int message_type,
			 mixed other)
{
    receive(msg);
}

//### Where is this used?  Is it needed?  What is it's purpose?
//Remote messages propogate just like an inside message by default
varargs void
receive_remote_msg(string msg, object array exclude, int message_type,
			mixed other)
{
    receive_inside_msg(msg, exclude, message_type, other);
}

//:FUNCTION receive_private_msg
//Receive a message sent just to us.
varargs void
receive_private_msg(string msg, int message_type, mixed other)
{
    receive(msg);
}
