/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** weather.c -- add weather interface for a room
**
** 950826, Deathblade: created (moved Yaynu's weather code)
*/

#include <daemons.h>

static private int weather;


void set_weather(int new_weather)
{
    weather = new_weather;
}
int query_weather()
{
    return weather;
}

string get_weather()
{
    if ( weather )
	return WEATHER_D->query_current_weather();

    return "";
}
