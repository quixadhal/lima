/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;

#define INACTIVE  0
#define RECORDING 1

private static int state;
private static int at_beginning = 1;
private static string transcript = "";

void setup() {
  set_adj( "fast forward", "rewind", "stop", "play", "record");
  set_id("recorder", "player", "button", "buttons", "rewind", "fast forward", "stop", "play", "record");
  set_long("It seems to be a tape recorder, and has the standard pannel of buttons.\nHowever, there seems to be no place to put a tape.");
  set_gettable(1);
}

string get_item_desc(string str)
{
  switch(str)
    {
    case "button":
      return "Which button do you mean?";
    case "record":
    case "record button":
      return sprintf("It's red and says, 'record'.%s",state == RECORDING ?
		     "  It's currently pressed down." : "");
    case "play":
    case "play button":
      return "It's a black button that says, 'play'.";
    case "stop":
    case "stop button":
      return "It's a black button that says, 'stop'.";
    case "fast forward":
    case "fast forward button":
      return "It's a black button that says, 'fast forward'.";
    case "rewind":
    case "rewind button":
      return "It's a black button that says, 'rewind'.";
    case "buttons":
      return "The front of the recorder consists of a pannel of buttons.\n"
	"There are buttons labeled: 'record', 'play', 'stop', 'fast forward', 'rewind'.\n";
    }
}

int press(string name)
{
  switch(name){
  case "buttons":
  case "button":
    write("Perhaps you could be a bit more specific than that.\n");
    return 1;
  case "record":
  case "record button":
    if(environment(this_object()) != environment(this_body()))
      {
	write("It won't pick up anything while you're holding it, you know,\n"
	      "except maybe the sound of your skin.\n");
      }
    if(at_beginning)
      transcript = "";
    state = RECORDING;
    this_body()->simple_action("$N $vstart recording on $p recorder.\n");
    at_beginning = 0;
    return 1;
  case "play":
  case "play button":
    if(!at_beginning)
      {
	write("You'll have to rewind the recorder, first.\n");
	return 1;
      }
    state = INACTIVE;
    this_body()->simple_action("$N $vpush the play button on $p recorder, and a transcript magically appears before $n, in lieu of an actual playback.\n");
    new(__DIR__ "transcript", transcript)->move(this_body());
    at_beginning = 0;
    return 1;
  case "rewind":
  case "rewind button":
    at_beginning = 1;
    state = 0;
    this_body()->simple_action("$N $vrewind $p recorder.\n");
    return 1;
  case "fast forward":
  case "fast forward button":
    at_beginning = 0;
    state = 0;
    this_body()->simple_action("$N fast $vforward $p recorder.\n");
    return 1;
  case "stop":
  case "stop button":
    if(!state)
      {
	write("The recorder is already stopped.\n");
	return 1;
      }
    state = 0;
    this_body()->simple_action("$N $vstop $p recorder.\n");
    return 1;
  default:
    return 0;
  }
}

void receive_outside_msg(string s)
{
  if(state)
    transcript += s;
}
