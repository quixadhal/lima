/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NON_ROOM;

int can_hold_water()
{
  return 0;
}

mixed enter_check()
{
  if(environment(this_body())!=environment(this_object())&&
     query_relation(this_body()) )
    return "You are already there.  Maybe you should stand up first.";
  return 1;
}


void mudlib_setup()
{
  ::mudlib_setup();
  add_relation("on",VERY_LARGE);
  
  add_method("sit on",this_object(),(: enter_check :),"$N $vsit on the $o,");
  add_method("stand",(:environment(this_object()):),1,"$N $vstands up");
}

