#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("How to make food");
  set_long(@MAY

Start as with any simple object, but inherit FOOD instead of OBJ

Add these extras within setup()

// Set the number of bites to finish the food
    set_num_eats(5);

// Set the message for eating a normal bite
    set_eat_action("$N $vtake a bite from an apple.");

// Set the message for eating the last bite
    set_last_eat_action("$N $veat the last of $p apple.");
}

MAY
  );
  set_exits( ([
    "pantry" : R + "pantry",
   ]) );
  set_objects( ([
    "/domains/std/apple" : 1,
   ]) );
}
