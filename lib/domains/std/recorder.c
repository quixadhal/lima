/* Do not remove the headers from this file! see /USAGE for more info. */
// Original by Rust
// Complete redesign by Ohara
// Future: Make recorder have real playback rather than transcript
// Future: Make recorder partially eraseable by a magnet
// Future: Allow stopping partway through a play
// Future: Make rewinds and fast forwards take time.

inherit CONTAINER;
inherit M_GETTABLE;

private nosave int at_beginning = 1;
private nosave string transcript = "";
private string current_button = "stop";


void setup()
{
    set_in_room_desc( "A recording device sits on the ground.");
    set_long( "It looks like a tape recorder, and it has the usual panel of buttons, but there's no place to put a tape in it.");
    set_id( "recorder", "player" );
    set_objects( ([
	"recorder/button/play" : 1,
	"recorder/button/record" : 1,
	"recorder/button/rewind" : 1,
	"recorder/button/fast forward" : 1,
	"recorder/button/stop" : 1,
	"recorder/panel" : 1,
      ]) );
    set_max_capacity( LARGE );
    set_hide_contents( 1 );
}


string get_current_button()
{
    return current_button;
}


string pop_up(string button)
{
    if( button == "stop" )
	return ".";
    else
	return ", and the " +button+ " button pops up with a small *click*.";
}


// Not really needed, but makes the design more obvious.
string push_down( string button )
{
    return "$N $vpush the " +button+ " button";
}


void pop_up_and_replace( string button )
{
    if( current_button == button && button != "stop" )
    {
	this_body()->simple_action( "$N $vstab at the " +button+ " button, only to find that it's already pressed down." );
	return;
    }
    this_body()->simple_action( push_down( button ) + pop_up( current_button ));
    current_button = button;
    call_other( this_object(), replace_string( button, " ", "_"));
}


void play()
{
    if( !(at_beginning && sizeof( transcript )))
    {
	this_body()->simple_action( "$N $vfind that the recorder appears to be at the end.");
	current_button = "stop";
	return;
    }
    object_event("The recorder spits out a transcript, in lieu of an actual playback" + pop_up( "play" ));
    new(__DIR__ "recorder/transcript", transcript)->move(this_body());
    at_beginning = 0;
}


void record()
{
    if(environment(this_object()) != environment(this_body()))
    {
	write("It won't pick up anything while you're holding it, you know, except maybe the sound of your skin.\n");
    }
    if(at_beginning)
	transcript = "\n";
    at_beginning = 0;
    object_event( "A soft sound can be heard as the recorder starts.");
}


void fast_forward()
{
    current_button = "stop";
    if( !( at_beginning && sizeof( transcript )))
    {
	object_event("The recorder, however, appears to already be at the end.");
	pop_up( "fast forward" );
	return;
    }
    at_beginning = 0;
    object_event( "There is a soft whirring from the machine.");
    call_out( (: "pop_up", "fast forward" :), 2);
}


void rewind()
{
    current_button = "stop";
    if( at_beginning || !sizeof(transcript))
    {
	object_event( "The recorder appears to already be at the beginning.");
	pop_up( "rewind" );
	return;
    }
    at_beginning = 1;
    object_event( "There is a soft whirring from the machine.");
    call_out( (: "pop_up", "rewind" :), 2);
}


void receive_outside_msg(string s)
{
    if( current_button == "record" )
	transcript += s;
}


mapping lpscript_attributes()
{
    return m_gettable::lpscript_attributes() + container::lpscript_attributes();
}

