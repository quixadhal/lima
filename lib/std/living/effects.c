/* Do not remove the headers from this file! see /USAGE for more info. */

// Effects system, idea based on the system used in Sorrows (ex Discworld?)

inherit CLASS_EFFECT;

private array effects = ({}); // Queue of effects to call
nosave private int tag; // Tag for call_out

//:MODULE
// Handles effects (ie delayed/repeating events)
// Each effect calls start_effect in the specified file when the effect starts
// Calls reinstate_effect when it restarts (at login etc)
// Calls do_effect periodically
// Calls end_effect when it ends
// Each of these functions takes arguments : effect sufferer, args, counter
//  where counter is the number of times it will repeat
//  args is a mixed set of args as appropriate to the effect

//:FUNCTION time_to_next_effect
// Returns time remaining in call_out to the next effect
int time_to_next_effect()
{
  mixed * callouts = call_out_info();
  foreach(mixed callout in callouts)
  {
    if(callout[0] == this_object())
      if(callout[1] == "next_effect")
        return callout[2];
  }
  return -1;
}

//:FUNCTION actual_period
// Converts period to an actual interval, with suitable randomisation
// Formats : int val : period = val
//           int array ({ fixed, var}) : period = fixed + random(var)
//           function : evaluate it
int actual_period( mixed val )
{
  if(intp(val))
    return val;

  if(arrayp(val))
    if(sizeof(val) == 2)
      if(intp(val[0]) && intp(val[1]))
        return val[0] + random(val[1]);

  if(functionp(val))
    return evaluate(val);

  return 0;
}

//###TODO add "special case" for time adjustment for removal of first effect
// where call_out remaining should be used in place of delay

//:FUNCTION remove_effect_at
// Removes effect from queue at appropriate point
void remove_effect_at(int pos)
{
  effects[pos-1]->delay += effects[pos]->delay;
  effects = effects[0..pos-1] + effects[pos+1..];
}

//:FUNCTION find_effect_index
// Return position of effect with specified object in queue
// Return -1 on failure
int find_effect_index(string ob)
{
  if(ob)
    for(int i = 0; i<sizeof(effects); i++)
    {
      if(effects[i]->effect_ob == ob)
        return i;
    }
  return -1;
}

//:FUNCTION find_effect_name_index
// Return position of effect with specified name in queue
// Return -1 on failure
int find_effect_name_index(string name)
{
  if(name)
    for(int i = 0; i<sizeof(effects); i++)
    {
      if(effects[i]->name == name)
        return i;
    }
  return -1;
}

//:FUNCTION find_effect_indexes_matching
// Return array of positions of effects (part-)matching specified name
// Return ({})
int array find_effect_indexes_matching(string name)
{
  int array res = ({});
  if(name)
    for(int i = 0; i<sizeof(effects); i++)
    {
      if(effects[i]->name[0..strlen(name)-1] == name)
        res += ({i});
    }
  return res;
}

//:FUNCTION find_effect
// Return effect with specified object in queue
// Return 0 on failure
int find_effect(string ob)
{
  int idx = find_effect_index(ob);
  if(idx>-1)
    return effects[idx];
  return 0;
}

//:FUNCTION query_effect_args
// Return args of specified effect
mixed query_effect_args(string ob)
{
  int idx = find_effect_index(ob);
  if(idx>-1)
    return effects[idx]->args;
  return 0;
}

//:FUNCTION remove_effect
// Locate effect matching the specified ob and remove it
// Return 1 on success, 0 on failure
int remove_effect(string ob)
{
  int index = find_effect_index(ob);
  if(index>-1)
    if(remove_effect_at(index))
      return 1;
  return 0;
}

//:FUNCTION remove_effect_named
// Locate effect matching the specified name and remove it
// Return 1 on success, 0 on failure
int remove_effect_named(string name)
{
  int index = find_effect_name_index(name);
  if(index>-1)
    if(remove_effect_at(index))
      return 1;
  return 0;
}

//:FUNCTION remove_effects_matching
// Locate effects matching the specified name and remove them
// Return 1 on success, 0 on failure
int remove_effects_matching(string name)
{
  int array matches = find_effect_indexes_matching(name);
  if(sizeof(matches))
  {
    foreach(int idx in sort_array(matches, -1))
      remove_effect_at(idx);
    return 1;
  }
  return 0;
}

//:FUNCTION insert_effect_at
// Inserts effect into queue at appropriate point
void insert_effect_at(class effect_class effect, int pos)
{
  effects = effects[0..pos-1] + ({ effect }) + effects[pos..];
}

//:FUNCTION insert_effect
// Finds appropriate point in queue to insert effect
// Adjusts delay to following effect
// Returns 1 on success, 0 on failure.
int insert_effect(class effect_class effect)
{
  class effect_class this_effect = new ( class effect_class );
  int cum_delay, prev_delay;

// If queue is empty, just put this effect in the queue
  if(!effects || !sizeof(effects))
  {
    effects = ({ effect });
    return 1;
  }

// Walk through queue, calculating cumulative delay
// and when this exceeds delay required for the effect to be inserted
// insert the effect
  for(int i=0; i<sizeof(effects); i++)
  {
    this_effect = effects[i];
    prev_delay = cum_delay;
    cum_delay += this_effect->delay;
    if(effect->delay<cum_delay)
    {
      insert_effect_at(effect,i);

// If there is an effect following this in the queue, 
// adjust its relative delay
      if(sizeof(effects)>i+1)
      {
        this_effect->delay -= (effect->delay - cum_delay);
        effects[i+1] = this_effect;
      }

// If there is an effect before this one in the queue,
// adjust the relative delay of this effect
      if(sizeof(effects)>1)
      {
        this_effect=effects[i];
        this_effect->delay -= prev_delay;
        effects[i] = this_effect;
      }
      return 1;
    }
  }
  return 0;
}

//:FUNCTION next_effect
// Call the function specified in the function at the head of the queue
// Move it to appropriate place in queue
// Then call out to next effect in the queue
void next_effect()
{
  class effect_class this_effect;
  object ob;

// Finish if the queue is empty
  if(!effects || !sizeof(effects))
    return;

// Do whatever the current head of the queue wants
  this_effect = effects[0];

  ob=load_object(this_effect->effect_ob);
  if(!ob)
    return;

// Remove head of queue
  effects = effects[1..];

// If counter is not zero, move current effect to later in the queue
  if(this_effect->counter!=0)
  {
// If counter >0, decrease it
    if(this_effect->counter>0)
      this_effect->counter --;

    this_effect->args = ob->do_effect(
        this_object(),
        this_effect->args,
        this_effect->counter
        );

// Calculate delay to next occurence
    this_effect->delay = actual_period(this_effect->interval);

// Put it into queue again
    insert_effect(this_effect);
  } else {
    ob->end_effect(
        this_object(),
        this_effect->args
        );
  }
// If there is anything in the queue, do a callout to it
  if(sizeof(effects))
  {
    this_effect = effects[0];
    tag = call_out("next_effect", this_effect->delay);
  } else
    tag = 0;

}

//:FUNCTION clear_effects
// Clears the effects queue
void clear_effects()
{
  object ob;
  foreach(class effect_class effect in effects)
  {
    ob = load_object(effect->effect_ob);
    if(ob)
      if(function_exists("end_effect", ob))
        ob->end_effect(
            this_object(),
            effect->args, 
            effect->counter
            );
  }
  effects = ({});
  tag = 0;
}

//:FUNCTION query_effects
// Returns copy of the effects queue
array query_effects() { return copy(effects); }

//:FUNCTION add_effect
// Adds the specified effect
// Usage: add_effect(string ob, mixed args, int repeats, mixed interval)
// repeats will default to ob->query_repeats()
// interval will default to ob->query_interval()
void add_effect(string ob, mixed args, int repeats, mixed interval)
{
  class effect_class this_effect = new(class effect_class);
  object obj = load_object(ob);
  if(!obj)
    return;
  if(undefinedp(repeats))
    repeats = ob->query_repeats();
  if(undefinedp(interval))
    interval = ob->query_interval();

  this_effect->effect_ob = ob;
  this_effect->name = ob->query_name();
  this_effect->args = args;
  this_effect->interval = interval;
  this_effect->counter = repeats;
  this_effect->delay = actual_period(interval);

  if(function_exists("start_effect", obj))
    this_effect->args = obj->start_effect(
        this_object(),
        args,
        repeats,
        interval
        );
  insert_effect(this_effect);
  if(!tag)
    tag = call_out("next_effect", this_effect->delay);
}

//:FUNCTION reinstate_effects
// Called on relogging to restart effects.
void reinstate_effects()
{
  object ob;
  foreach(class effect_class effect in effects)
  {
    ob = load_object(effect->effect_ob);
    if(!ob)
      continue;
    if(function_exists("reinstate_effect", ob))
      effect->args = ob->reinstate_effect(
          this_object(),
          effect->args,
          effect->counter, 
          );
  }
  next_effect();
}
