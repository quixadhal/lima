/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
// This module should be inherited into ammunition,
// to interact with the "ready" verb which prepares for firing

//:FUNCTION do_ready
// Adds the object to the "ready" array of ammunition
void do_ready()
{
  this_body()->add_readied(this_object());
}

//:FUNCTION do_unready
// Removes the object from the "ready" array of ammunition
void do_unready()
{
  this_body()->remove_readied(this_object());
}


// TODO - add some checks - eg not already readied
mixed direct_ready_obj()
{
  object * readied = this_body()->query_readied();

  if(readied && member_array(this_object(), readied)>-1)
    return "#Already readied.";
  return this_object()->need_to_have();
}

mixed direct_unready_obj()
{
  object * readied = this_body()->query_readied();

  if(readied && member_array(this_object(), readied) == -1)
    return "#Not readied.";
  return this_object()->need_to_have();
}
