/* Do not remove the headers from this file! see /USAGE for more info. */

void catch_tell(string s)
{
  object env = environment(this_object());

  if(!environment(this_body()) || environment(this_body()) != env)
    return;

  tell_room(environment(env), s, ({env}));
}

int move(object s)
{
  if(environment(this_object()))
    {
      return 0;
    }
  move_object(s);
  return 1;
}


string query_in_room_desc()
{
  return "";
}
