/* Do not remove the headers from this file! see /USAGE for more info. */

// base disease

// args to be an array:
// contagiousness (% chance of spreading each repeat)
// .. others .. (eg strength, cure-difficulty)
// original number of repeats
// period
// Initially set up with just contagiousness .. others,
// and repeats/period added in the start_effect call

// Overload this for each disease
void start_disease(object player)
{
  player->simple_action("$N $vstart to suffer from the disease.");
}

// NOTE - Ensure the return of args + ({ counter, period }) is retained -
// it is required for disease spreading
mixed start_effect(object player, mixed args, int counter, mixed period)
{
  start_disease(player);
  return args + ({ counter, period });
}

// Overload this for each disease
// MUST return args (modified as appropriate)
mixed disease_effect(object player, mixed args)
{
  player->simple_action("$N $vcontinue to suffer from the disease.");
  return args;
}

mixed do_effect(object player, mixed args, int counter)
{
  object env = environment(player);
  if(env)
  {
    foreach(object liv in all_inventory(env) - ({player}))
    {
// Potential spread of the disease to others in the room
      if(random(100)<args[0])
      {
        liv->add_effect(base_name(this_object()),
            ({args[0..<3]}),
            args[<2],
            args[<1],
            );
      }
    }
  }
  return disease_effect(player, args);
}

mixed end_effect(object player, mixed args, int counter)
{
  player->simple_action("$P suffering ends as the disease leaves.");
  return args;
} 
