/* Do not remove the headers from this file! see /USAGE for more info. */

int environment_can_hear()
{
  object env = environment();

  return env && !env->cant_hear_contents();
}

// Inside messages propogate upward and downward, but we have no downward
void receive_inside_msg(string msg, object array exclude, int message_type, 
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

// Outside messages propogate downward, so just receive
void receive_outside_msg(string msg, object array exclude, int message_type,
			 mixed other)
{
  receive(msg);
}

//Remote messages propogate just like an inside message by defauly
void receive_remote_msg(string msg, object array exclude, int message_type,
			mixed other)
{
  receive_inside_msg(msg, exclude, message_type, other);
}

void receive_private_msg(string msg, int message_type, mixed other)
{
  receive(msg);
}
