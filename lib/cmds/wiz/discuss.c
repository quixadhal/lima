/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE discuss <thing>
//
//Invokes the annotation menu, allowing annotations to be added to objects,
//and subsequently viewed and deleted.

inherit CMD;

private void main(mixed *arg)
{
  if(userp(arg[0]) || member_array(arg[0],bodies()) != -1)
  {
    out("You can't discuss a user, just objects.\n");
    return;
  }
  new(ANNO_MENU, arg[0])->start_menu();
}
