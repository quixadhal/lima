/* Do not remove the headers from this file! see /USAGE for more info. */

// common cold - example disease

inherit __DIR__ "base_disease";


string query_name() { return "disease.cold"; }

// Default number of repeats
int query_repeats(){ return 100; }

// Default period between repeats
int query_interval() { return 10; }

// Overload this for each disease
void start_disease(object player)
{
  player->simple_action("$N $vstart to sniffle.");
}

// Overload this for each disease
// MUST return args (modified as appropriate)
mixed disease_effect(object player, mixed args)
{
  player->simple_action("$N $vcough and $vsneeze.");
  return args;
}

mixed end_effect(object player, mixed args, int counter)
{
  player->simple_action("$P $vsigh with relief as the cold departs.");
  return args;
} 
