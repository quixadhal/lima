/* Do not remove the headers from this file! see /USAGE for more info. */

string short();
string the_short();

private static string on_enter_msg;
private static string on_exit_msg;

void set_on_enter_msg(string s)
{
  on_enter_msg = s;
}

void set_on_exit_msg(string s)
{
  on_exit_msg = s;
}

string get_on_enter_msg()
{
  return on_enter_msg;
}

string get_on_exit_msg()
{
  return on_exit_msg;
}

string handle_exit_msgs(object last_loc)
{
  return get_on_enter_msg() || "$N $venter "+the_short()+".\n";
}

int direct_enter_obj()
{
  return 1;
}

int do_enter()
{
  if(environment(this_body()) == this_object())
    {
      write("You are already there.\n");
      return 1;
    }
  this_body()->move_to(file_name(this_object()), short());
}

int direct_get_in_obj()
{
  return 1;
}

int get_in_obj()
{
  do_enter();
}

int direct_exit_obj()
{
  return 1;
}


mixed exit()
{
  return get_on_exit_msg() || "$N $vexit " + the_short() + ".\n";
}

