/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;


private void main(mixed *arg)
{
  if(userp(arg[0]) || member_array(arg[0],bodies()) != -1)
    {
      write("You can't discuss a user, just objects.\n");
      return;
    }
  new("/obj/annotationmenu", arg[0])->start_menu();
}
