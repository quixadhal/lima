/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stand.c
*/


inherit VERB_OB;

//### for now.  We need a better system to tell check_condition() what to
//### ignore.
mixed can_stand_up() {
    return 1;
}

mixed can_stand() {
    return 1;
}

void do_stand()
{
  mixed s;
  string err;

    if (this_body()->query_prone()) {
	if (this_body()->get_up())
	    this_body()->simple_action("$N $vget back up on $p feet.");
	return;
    }
  if(s = environment(this_body())->stand())
    {
      if(!stringp(err=this_body()->move(environment(environment(this_body())))))
	{
	  if(stringp(s))
	    this_body()->simple_action(s);
	  else
	    this_body()->simple_action("$N $vstand up.");
	}
      else
	{
	  write(err+"\n");
	}
    }
  else
    {
      if(environment(environment(this_body())))
	{
	  write("You can't stand up.\n");
	}
      else
	{
	  write("You are already standing.\n");
	}
    }
} 

void do_stand_up()
{
  do_stand();
}

array query_verb_info()
{
    return ({ ({ "" }), ({}), ({ "up" }), ({ "get" }) });
}


