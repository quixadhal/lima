/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BASE_ROOM;
inherit __DIR__ "room/weather";


int is_outdoors ()
{
  return 1;
}

void create ()
{
  ::create ();
  add_id_no_plural ("ground");
}

// Make weather show something....

string get_extra_long ()
{
  string	weather_str;
  string	other_extra_long;

  weather_str = get_weather ();
  if (!weather_str || !strlen (weather_str))
    {
      return ::get_extra_long ();
    }
  else
    {
      other_extra_long = ::get_extra_long ();
      if (!other_extra_long)
	{
	  return weather_str;
	}
      else
	{
	  return other_extra_long + weather_str + "\n";
	}
    }
}
