/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: heal <playername>
//
//Completely heals the targetted player.

inherit CMD;

private void main(string arg)
{
   object target;

   if(!arg || arg == "me" || arg == "myself")
   {
      this_body()->simple_action("$N $vclose $p eyes and $vglow warmly, healing
$no.");
      this_body()->heal_all();
      return;
   }
   if(!target = find_body(arg))
   {
      write("You can't heal that.\n");
      return;
   }
   if(target == this_body())
   {
      this_body()->simple_action("$N $vclose $p eyes and $vglow warmly, healing
$no.");
      this_body()->heal_all();
      return;
   }
   target->heal_all();
   this_body()->targetted_action("$N $vraise $p arms and $vheal $t.", target);
   return;
 }
