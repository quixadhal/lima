/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_DAMAGE_SOURCE;
inherit M_GETTABLE;
inherit M_MESSAGES;
inherit M_READY;

// Called out when the ammunition has been fired, and has hit/missed
// Overload as necessary, but should typically either remove the item
// or move it to target's environment().
void finish_discharge(object target)
{
  this_object()->move(environment(target));
}

// Called when the ammunition is discharged.
// Overload as needed, but normally calls out finish_discharge(),
// which will thus occur after the ammunition has hit/missed the target
void discharge(object target)
{
  call_out( (: finish_discharge :), 0, target );
}
