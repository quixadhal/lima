#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Some improvements to a simple shopkeeper");
    set_long(@MAY

Start with the simple shopkeeper as specified in the previous room

Within the setup, there are many options available.
// Determine what type of currency we'll use
    set_currency_type("gold");

// Sets shopkeeper to sell things (0 for nothing)
    set_for_sale(1);

// Sets shopkeeper to buy things (0 for nothing)
    set_will_buy(1);

// Set basis for selling price (ie value will be multiplied by this)
// Default = 1.5
    set_cost_multiplicator(1.5);
}
MAY
    );
    set_exits( ([
	"finish" : R "zoo",
	"backward" : R "shopkeeper01",
      ]) );
    set_objects( ([
    "/domains/std/shopkeeper" : 1,
      ]) );
}
