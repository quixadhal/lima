/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

private void main(string arg)
{
   object target;

   if(!arg || arg == "me" || arg == "myself")
   {
      if(!this_body()->query_ghost())
      {
         write("You're not dead yet!\n");
         return;
      }
      this_body()->simple_action("$N $vreturn to the world of the living.\n");
      this_body()->reincarnate();
      return;
   }
   else
      target = find_body(arg);

   if(!target)
   {
      write("You can't resurrect that.\n");
      return;
   }

   if(target->query_ghost())
   {
      this_body()->targetted_action("$N $vbring $t back to life.\n", target);
      target->reincarnate();
   }
   else
   {
      this_body()->targetted_action("$N $vget the impression $t doesn't need any help.\n",
                             target);
   }
}

