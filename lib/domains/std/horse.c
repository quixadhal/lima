/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** horse.c
**
** Example of a mount.
**
** 951220, Rust: Created.
*/

inherit VEHICLE;
inherit M_MOUNTABLE;

void create()
{
    ::create();
    set_adj("ugly");
    set_id("horse");
    set_long("Damn, the horse is ugly!");
    set_max_capacity(VERY_LARGE*2);
    set_preposition("on");
    // So people will see: Sitting on the horse you see Rust...
    set_primary_verb("sitting");
    set_in_room_desc("There is an ugly horse here.");
    set_get_on_msg("$N $vget onto the ugly horse.\n");
    set_get_off_msg("$N $vdismount from the ugly horse.\n");

// We could opt for simple messages, and uncomment these
// 2 lines, but we can also go for more complex msgs (see below).
//    set_arrival_msg("The $N $vtrot off $o.\n");
//    set_departure_msg("A $N $vtrot in.\n");
}


string get_arrival_msg()
{
  string riders;

  if(!(riders = get_riders_as_string()))
    return 0;

  return capitalize(the_short()) + " trots off, carrying " + riders +".\n";
}

string get_departure_msg()
{
  string riders;

  if(!(riders = get_riders_as_string()))
    return 0;

  if(sizeof(get_riders()) != 1)
    {
      return riders + " ride in on " + a_short() + ".\n";
    }
  else
    {
      return riders + " rides in on " + a_short() + ".\n";
    }
}
