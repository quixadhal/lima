/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Yaynu @ Lima Bean (on 950815)
** Very Basic Weather Daemon.
*/

#define WEATHER_PERIOD	120	/* Weather duration. */

/*
** The current weather types
*/
nosave private string * types = ({ "sunny", "rainy", "foggy", "snowy" });
nosave private string * type_nouns = ({ "sun", "rain", "fog", "snow" });

/*
** Globals.
*/
nosave private int weather_state;


string show_weather_change(int last_state)
{
    string buf = "\n";

    if (last_state == 0) buf+="Thick clouds form in the sky.\n";

    switch(random(5)){
    case 0:
	buf+="The "+type_nouns[last_state]+" vanishes. ";
    case 1:
	buf+="The "+types[last_state]+" weather slowly changes. ";
	break;
    case 2:
	buf+="The "+type_nouns[last_state]+" disappears. ";
    case 3:
	buf+="The "+types[last_state]+" weather comes to a end. ";
	break;
    default:
	buf+="The weather suddenly changes. ";
    }

    switch(random(3)){
    case 0:
	buf+= "It becomes very "+types[weather_state]+".\n";
	break;
    default:
	buf+= "The day becomes "+types[weather_state]+".\n";
	break;
    }

    return buf;
}

private void change_weather()
{
    int last_state;
    string buf_me_once;
    object body;

    remove_call_out();	// all call_outs

    last_state = weather_state;
    weather_state = random(sizeof(types));
    if ( weather_state != last_state )
    {
	buf_me_once = show_weather_change(last_state);
	foreach ( body in bodies() - ({ 0 }) )
	{
	    if ( environment(body) && environment(body)->query_weather() )
		tell(body, buf_me_once);
	}
    }

    call_out((: change_weather :), WEATHER_PERIOD);
}

string query_current_weather(){
   if(!weather_state)
        return ("The sun is high in the sky. It is quite bright.");
   if(weather_state == 2)
        return("There are patches of grey fog floating around.");
   return("There is "+type_nouns[weather_state]+" falling down around you.");
}

void create() {
  change_weather();
}
