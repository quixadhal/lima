/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stand.c
*/


inherit VERB_OB;


void do_stand()
{
  mixed s;
  string err;

  if(s = environment(this_body())->stand())
    {
      if(!stringp(err=this_body()->move(environment(environment(this_body())))))
	{
	  if(stringp(s))
	    this_body()->simple_action(s);
	  else
	    this_body()->simple_action("$N $vstand up.\n");
	}
      else
	{
	  write(err+"\n");
	}
    }
  else
    {
      write("You are already standing.\n");
    }
} 

void do_stand_up()
{
  do_stand();
}

int can_stand()
{
  return 1;
}

int can_stand_up()
{
  return 1;
}

mixed array query_verb_info()
{
    return ({ ({ "", "up" }) });
}


