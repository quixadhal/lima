/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** dismount.c
*/


inherit VERB_OB;


void do_dismount(object what)
{
  mixed s;
  string err;

  if(!objectp(what))
    what = environment(this_body());

  if(what != environment(this_body()))
    {
      write("You're not on it.\n");
      return;
    }
  if(s = what->dismount())
    {
      if(!stringp(err=this_body()->move(environment(what))))
	{
	  if(stringp(s))
	    this_body()->simple_action(s);
	  else
	    this_body()->simple_action("$N $vget off " + what->the_short()+
				       ".\n");
	}
      else
	{
	  write(err+"\n");
	}
    }
  else
    {
      if(!environment(environment(this_body())))
	write("You are already standing.\n");
      else
	write("You're have no mount of which to speak.\n");
    }
} 

void do_dismount_obj(object o)
{
  do_dismount(o);
}

int can_dismount_obj()
{
  return 1;
}

int can_dismount()
{
  return 1;
}


mixed array query_verb_info()
{
    return ({ ({ "", "OBJ" }) });
}


