/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NON_ROOM;

string handle_exit_msgs(object last_loc)
{
  return "$N $vget on "+the_short() + ".\n";
}

int can_hold_water()
{
  return 0;
}

int direct_sit_on_obj()
{
  return 1;
}

int direct_sit_in_obj()
{
  return 1;
}

void sit()
{
  do_enter();
}

int stand()
{
  if(environment(this_body()) != this_object())
    return 0;
  
  return 1;
}

string inventory_header()
{
  return "On " + the_short() + "is:";
}
