/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "limbs";

string number_word(int);       /* M_GRAMMAR */
string number_of(int, string);

/* Value is an array of numbers, indicating the size of the wounds.  Note
 * that the sum of wounds[limb] should ALWAYS be health[limb]->max_health
 * minus health[limb]->health.
 *
 * Limbs in perfect condition and disabled limbs are not in the mapping.
 */
private mapping wounds = ([]);

void set_max_limb_health(string limb, int x)
{
   ::set_max_limb_health(limb, x);
   map_delete(wounds, limb);
   if(!sizeof(wounds))
      remove_call_out("health_periodic");
}

void disable_limb(string limb)
{
   ::disable_limb(limb);
   map_delete(wounds, limb);
   if(!sizeof(wounds))
      remove_call_out("health_periodic");
}

varargs int hurt_us(int x, string limb)
{
   int ret;

   if(!limb)
      limb = query_random_limb();
   ret = ::hurt_us(x, limb);

   /* ret is the amount of damage actually done; remember to check if we
      disabled the limb */

   if(ret && query_health(limb))
   {
      if(!sizeof(wounds))
         call_out("health_periodic", 15);

      if(wounds[limb])
      {
         int n = sizeof(wounds[limb]);

         /* To prevent huge lists of wounds when chinking away with
            small amounts of damage on a really large monster, we coalesce
            wounds after about 5-10 of them */
         if(n > random(5) + 5)
            wounds[limb][random(n)] += ret;
         else
            wounds[limb] += ({ ret });
      }
      else
         wounds[limb] = ({ ret });
   }

   return ret;
}

void heal_limb(string limb, int x)
{
   int i = 0;
   array tmp = wounds[limb];
   int n = sizeof(tmp);
   int left = x;

   ::heal_limb(limb, x);
   if(n == 0)
      return;

   /* start healing with the first wound, and continue till we run out of
      'healing' */
   while(i < n)
   {
      if(left >= tmp[i])
      {
         left -= tmp[i];
         i++;
      }
      else
      {
         tmp[i] -= left;
         break;
      }
   }
   if(i < n)
      wounds[limb] = tmp[i..];
   else map_delete(wounds, limb);

   /* i == number of wounds healed; n == total number before */
   if(i)
   {
      if(n == 1)
         simple_action("The wound on $p " + limb + " has healed.\n");
      else if(i == n)
         simple_action("All of the wounds on $p " + limb + " have healed.\n");
      else
         simple_action(capitalize(number_word(i)) + " of the " + number_word(n) + " wounds on $p " + limb + " have healed.\n");
   }
}

void heal_all()
{
   wounds = ([]);
   ::heal_all();
}

void kill_us()
{
   remove_call_out("health_periodic");
   ::kill_us();
}

// Unfortunately this can't be done lazily since it has side effects
void health_periodic()
{
   string limb;
   array w;
   int i, n, severity, delta;

   /* pick a random wound, and make it better or worse by up to 20%;
    * the function is centered on zero, but remember that normal healing
    * is causing us to heal at ~1 point per heal_rate seconds, so the
    * overall effect is biased towards healing.
    */
   if(sizeof(wounds) == 0)
      return;
   limb = choice(keys(wounds));
   w = wounds[limb];
   n = sizeof(w);
   i = random(n);
   severity = w[i];
   delta = fuzzy_divide(random(2*severity+1) - severity, 5);

   call_out("health_periodic", 15);

   if(delta == 0)
      return;
    
   if(delta < 0)
   {
      if(n == 1)
         simple_action("The wound on $p " + limb + " looks a bit worse.");
      else
         simple_action("One of the wounds on $p " + limb + " looks a bit worse.");

      wounds[limb][i] += ::hurt_us(-delta, limb);
   }
   else
   {
      if(n == 1)
         simple_action("The wound on $p " + limb + " looks a bit better.");
      else
         simple_action("One of the wounds on $p " + limb + " looks a bit better.");

      wounds[limb][i] -= delta;

      ::heal_limb(limb, delta);
   }
}

void reincarnate()
{
   foreach(string s in query_vital_limbs())
      map_delete(wounds, s);
   ::reincarnate();
   health_periodic();
}

void health_status()
{
   foreach (string limb in keys(wounds))
   {
      int h = query_health(limb);
      int mh = query_max_health(limb);

      if(wounds[limb])
         printf("%-10s %3i (%3i%%) [%s]\n", limb, h, h * 100 / mh,
                implode(map(wounds[limb], (: $1 + "" :)), ","));
   }
}

string array query_all_wounds()
{
   return keys(wounds);
}

array query_wounds(string limb)
{
   return wounds[limb];
}

string diagnose_msg(string limb)
{
   mapping stuff = ([ ]);
   int array my_wounds = query_wounds(limb);
   int max = query_max_health(limb);
   string array types = ({ });

   if(!query_health(limb))
      return "$P " + limb + " is completely disabled!\n";
   if(!my_wounds)
      return "$P " + limb + " is in good condition.\n";
   my_wounds = sort_array(my_wounds, 1);
   foreach(int x in my_wounds)
   {
      switch(x * 100 / max)
      {
         case 0..10:   stuff["bruise"]++;     break;
         case 11..20:  stuff["scratch"]++;    break;
         case 21..30:  stuff["cut"]++;        break;
         case 31..50:  stuff["wound"]++;      break;
         case 51..71:  stuff["injury"]++;     break;
         case 72..85:  stuff["gash"]++;       break;
         case 86..100: stuff["mutilation"]++; break;
      }
   }

   foreach(string s in keys(stuff))
      types += ({ number_of(stuff[s], s) });
   return "$P " + limb + " $vhave sustained " + format_list(types, 0) + ".\n";
}

varargs string diagnose(string limb)
{
   string array limbs = query_limbs();
   string ret = "";

   if(!limb)
   {
      foreach(string s in limbs)
         ret += diagnose_msg(s);
   }
   else ret += diagnose_msg(limb);

   return ret;
}

