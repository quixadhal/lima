/* Do not remove the headers from this file! see /USAGE for more info. */

int query_ghost();

private nosave int stunned, asleep, chance;

int query_stunned()
{
  if(stunned > time())
    return 1;
  return 0;
}

int query_asleep()
{
   return asleep;
}

void stun()
{
   stunned = time() + 5;
}

void wake_up()
{
   if(asleep)
   {
      asleep = 0;
      stunned = time() + 5;
   }
}

void knock_out()
{
   asleep = 1;
   chance = 0;
}

mixed check_condition(int urgent)
{
   if(query_ghost())
   {
      stunned = 0;
      asleep = 0;
      return 0;
   }

   if(urgent && stunned > time())
      return "You $vare still recovering from that last blow, ";

   if(asleep)
   {
      if(urgent)
      {
         if(random(5) <= chance++)
         {
            wake_up();
         }
         else
         {
            return "You try desperately to wake up, but fail.\n";
         }
      }
      else
      {
         return "But you are asleep!\n";
      }
   }

   return 0;
}
