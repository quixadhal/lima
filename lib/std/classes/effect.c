/* Do not remove the headers from this file! see /USAGE for more info. */

// For delayed / repeating effects

class effect_class
{
  int delay;         // delay until called again
  string effect_ob;  // effect object to be used
  string name;       // name to identify/classify it (eg disease.cold)
  mixed interval;    // evaluate for next delay
  mixed args;        // arguments to be passed in call
  int counter;       // counter for number of repeats left (-1 for infinite)
}
