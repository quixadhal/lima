/* Do not remove the headers from this file! see /USAGE for more info. */

private static string mount_msg;
private static string dismount_msg;

string the_short();
string short();


string show_exits()
{
    object env = environment();
    return (env ? env->show_exits() : "none");
}


static void set_get_on_msg(string s)
{
  mount_msg = s;
}

static void set_get_off_msg(string s)
{
  dismount_msg = s;
}

string query_get_on_msg()
{
  return mount_msg;
}

string query_get_off_msg()
{
  return dismount_msg;
}

string handle_exit_msgs(object last_loc)
{
  return query_get_on_msg() || "$N $vget on "+the_short()+".\n";
}

int direct_sit_on_obj()
{
  return 1;
}

private void mount_it_already()
{
  if(environment(this_body()) == this_object())
    {
      write("You are already there.\n");
      return;
    }
  this_body()->move_to(file_name(this_object()), short());
}

void sit()
{
  mount_it_already();
}

mixed stand()
{
  return query_get_off_msg() || "$N $vget off " + the_short() + ".\n";
}

int direct_mount_obj()
{
  return 1;
}

void mount()
{
  mount_it_already();
}

void get_on()
{
  mount_it_already();
}

int direct_dismount_obj()
{
  return 1;
}

int dismount()
{
  if(environment(this_body()) != this_object())
    return 0;
  
  return stand();
}

int get_off()
{
  if(environment(this_body()) != this_object())
    return 0;
  
  return stand();
}

// It doesn't make much sense to enter a horse.
int direct_enter_obj()
{
  return 0;
}

int direct_get_off_obj()
{
  return 1;
}

int direct_get_on_obj()
{
  return 1;
}

