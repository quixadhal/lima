/* Do not remove the headers from this file! see /USAGE for more info. */



int direct_knock_at_obj()
{
  return 1;
}

int direct_knock_on_obj()
{
  return 1;
}

void knock_knock(string name)
{
  this_body()->targetted_action("$N $vrap on $t.\n", this_object());
}
