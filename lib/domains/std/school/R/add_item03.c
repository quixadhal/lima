#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Add_items reacting to other verbs");
    set_long(@MAY

Add actions can react to other verbs than look.
Extend the description mapping to include the other verbs and responses.
Verbs for which this works are get, pull, press, search, listen, smell, read.

  add_item( "desk",
    (["look" : "It's just an ordinary wooden desk.\n",
      "get": "What an odd idea - it's far too heavy.\n",
      "pull": "It's too heavy to move.\n",
      "press": "You press it but nothing happens.\n",
      "smell": "You can smell something dead in one of the drawers.\n",
      "read": "It says \"Quiet, Genius at work\".\n",
      ]) );
MAY
    );

  add_item( "desk",
    (["look" : "It's just an ordinary wooden desk.\n",
      "get": "What an odd idea - it's far too heavy.\n",
      "pull": "It's too heavy to move.\n",
      "press": "You press it but nothing happens.\n",
      "smell": "You can smell something dead in one of the drawers.\n",
      "read": "It says \"Quiet, Genius at work\".\n",
      ]) );

    set_exits( ([
	"backward" : R "add_item02",
	"finish" : R "room",
      ]) );
    set_objects( ([
      ]) );
}
