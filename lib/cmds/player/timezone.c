/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
//$$see :time
//USAGE timezone <value>
//
//Sets the player's time difference (relative to GMT).
//This lets the mud display your local time as part of the time command.
//It's probably easier to look at a clock, but this does allow you to see
//at a glance if the mud's clock is wrong.

inherit CMD;

private void main(string val)
{
  float actual = to_float(val);
  if(!strlen(val))
  {
    actual=to_float(this_body()->query_tz());
    outf( "Your timezone setting is %f\n", actual );
    return;
  }
  if(actual)
  {
    this_body()->set_tz(actual);
    actual=to_float(this_body()->query_tz());
    outf( "Your new timezone setting is %f\n", actual );
    return;
  }
// Add handling for strings - eg "EST" -> -5.0
  write("Usage timezone <value>\n");
  write ("valid values are GMT offsets in hours, from -12 to +12.\n");
}