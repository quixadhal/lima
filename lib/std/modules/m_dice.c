/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
// This module handles all dice rolling functions

//:FUNCTION roll_dice
// Roll x number of dice with y sides. 
// Not all gaming systems use just the total of the dice, so each result is 
// returned in an array.
int array roll_dice(int num_dice, int num_sides)
{
  int i;
  int array results=({});

  for(i=0;i<num_dice;i++) {
    results+=({random(num_sides)+1});
  }
  return results;
}
