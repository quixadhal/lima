#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a banker");
    set_long(@MAY

Start with a file similar to simple npc.
Inheriting M_VENDOR as well.

Additional items needed in setup() :

//Short name (or id) for the bank
  set_bank_id("lima");

//Formal name for the bank
  set_bank_name("The Imperial Bank of the Bean");

//Type of currency it keeps accounts in
  set_currency_type("dollar");

//Fee charged it your cash is in the 'wrong' currency (%)
  set_exchange_fee(5);

}
MAY
    );
    set_exits( ([
	"monsters" : R "zoo",
      ]) );
    set_objects( ([
    "/domains/std/bank_accountant" : 1,
      ]) );
}
