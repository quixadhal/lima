/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove these headers see /USAGE for more info */

//:MODULE
//This module can be used to make objects decay after a set
//amount of time. You may also have an object decay in stages.
/*
** Coded by Kinison@Private Idaho - June 18, 1996
*/

private int num_decays, decay_time;
private mixed decay_action;
private mixed last_decay_action;
private int tag;
private int auto_remove;

void remove();

//:FUNCTION set_decay_action
//set_decay_action(string) causes 'string' to be printed this
//object decays.  set_decay_action(function) causes the function
//to be called instead.
void set_decay_action(mixed action)
{
   decay_action = action;
}

//:FUNCTION set_last_decay_action
//same as set_decay_action but only on the last decay.
void set_last_decay_action(mixed action)
{
   last_decay_action = action;
}

//:FUNCTION set_num_decays
//This sets the number of stages to decay in.
int set_num_decays(int num)
{
   return (num_decays = num);
}


void set_decay_auto_remove(int i)
{
   auto_remove = i;
}

int query_num_decays()
{
   return num_decays;
}

void start_decay()
{
   tag = call_out("decay_it", decay_time);
}

//:FUNCTION set_decay_time 
//Set the length of time that it takes to decay this item.
//setting this makes the item decayable, not setting this varriable will not
//make it decay.
varargs void set_decay_time(int num, int dont_autostart)
{
   if(num)
   {
      decay_time = num;
      if(!dont_autostart)
         start_decay();
   }
}

//Decay at this time and remove the object if at the last decay.
void decay_it()
{
   mixed action;

   if(num_decays == 1 && last_decay_action)
      action = last_decay_action;
   else
      action = decay_action;

   if(stringp(action))
      object_event(action);
   else
      evaluate(action);

   num_decays--;

   if(num_decays)
      tag = call_out("decay_it", decay_time);
   else if(auto_remove)
      remove();
}

void stop_decay()
{
   remove_call_out(tag);
}
