/* Do not remove the headers from this file! see /USAGE for more info. */

/* $Id: accountant.c,v 1.4 1998/02/25 01:45:48 monica Exp $ */

/* created/adjusted 9-Feb-98 by MonicaS */

#include <menu.h>

inherit LIVING;
inherit M_ACTIONS;
inherit MENUS;

MENU toplevel;
private string bank_id = "unknown", bank_name = "Unknown Bank";
private string deposit_currency = "gold";
private float exchange_fee = 5.0; // in percent
nosave object player;
nosave int was_busy;

//:MODULE
//The accountant inheritable includes everything that is needed for a bank.
//Only set_currency_type(), set_exchange_fee(), set_bank_id() and
//set_bank_name() are needed to define the bank actions. It inherits LIVING.

//:FUNCTION set_currency_type
//Sets the type of currency the bank will deposit
void set_currency_type(string currency) {
  if(!MONEY_D->query_exchange_rate(currency))
    catch("Invalid currency set in accountant!\n");
  deposit_currency = currency;
}

//:FUNCTION query_currency_type
//Queries the type of currency the bank deposits
string query_currency_type() {
  return deposit_currency;
}

//:FUNCTION set_exchange_fee
//The fee (in percent) is deducted when exchanging different currencies
void set_exchange_fee(float fee) {
  exchange_fee = fee;
}

//:FUNCTION query_exchange_fee
float query_exchange_fee() {
  return exchange_fee;
}

//:FUNCTION set_bank_id
//The id is the identifier used with ACCOUNT_D for deposits.
void set_bank_id(string id) {
  bank_id = id;
}

//:FUNCTION set_bank_name
//The name is used in the texts for the players
void set_bank_name(string name) {
  bank_name = name;
}

void show_money() {
  string array currencies = ({ });
  mapping money = this_body()->query_money();

  if ( sizeof(money) == 0 ) {
    write("Sorry, you have no money.\n");
  } 
  else {
    write("You have the following types of money:\n");
    foreach (string type in keys(money)) {
      currencies |= ({ MONEY_D->query_currency(type) });
    }
    foreach (string currency in currencies) {
      printf("   %-13s%s\n", capitalize(currency)+":",
	     MONEY_D->currency_to_string(money, currency)); 
    }
  }
  printf("Your account balance is %s.\n\n",
	 MONEY_D->currency_to_string(
	   ACCOUNT_D->query_account(bank_id, player), deposit_currency));
}

void show_rates() {
  string * types = MONEY_D->query_currency_types();
  
  if ( sizeof(types) == 0 )
    write("Sorry, no currencies are defined.\n");
  else {
    float base_rate = to_float(MONEY_D->query_exchange_rate(deposit_currency));
    printf("   %-12s%-12s%12s\n   %*'-'s\n", "Currency", "Denomination",
	   "Value", 72, "");
    foreach (string type in types) {
      foreach (string denomination in MONEY_D->query_denominations(type)) {
	float value = to_float(MONEY_D->query_exchange_rate(type)) 
	  * MONEY_D->query_factor(denomination) / base_rate;
	printf("   %-12s%-12s%12.4f %s\n", capitalize(type), denomination,
	       value, 
	       value == 1.0 ? deposit_currency : 
	       MONEY_D->query_plural(deposit_currency));
      }
    }
  }
}

void exchange4b(string currency1, string denomination2, string answer) {
  string currency2;
  float rate1, rate2, factor, amount1;
  int amount2 = to_int(answer);
  mapping array money_info;
  
  if (amount2 > 0) {
    currency2 = MONEY_D->query_currency(denomination2);
    rate1 = to_float(MONEY_D->query_exchange_rate(currency1));
    rate2 = to_float(MONEY_D->query_exchange_rate(currency2))
      * MONEY_D->query_factor(denomination2);
    if (currency1 == currency2)
      factor = 1.0;
    else
      factor = 1.0 - (exchange_fee / 100.0);
    amount1 = to_float(amount2) * rate2 / rate1 / factor;
    if (player->query_amt_currency(currency1) >= amount1) { 
      money_info = MONEY_D->handle_subtract_money(player, amount1, currency1);
      player->add_money(denomination2, amount2);
      write("You exchange "
	    +MONEY_D->currency_to_string(money_info[0], currency1)+" into "
	    +MONEY_D->denomination_to_string(amount2, denomination2)
	    +(sizeof(money_info[1])?" and get "
	      +MONEY_D->currency_to_string(money_info[1], currency1)
	      +" as change":"")
	    +".\n");
    }
    else
      write("You don't have enough cash.\n");
  } 
}

void exchange3b(string currency1, string denomination2) {
  string currency2;
  float rate1, rate2, factor;
  int amount2;

  if (MONEY_D->is_denomination(denomination2)) {
    currency2 = MONEY_D->query_currency(denomination2);
    rate1 = to_float(MONEY_D->query_exchange_rate(currency1));
    rate2 = to_float(MONEY_D->query_exchange_rate(currency2))
      * MONEY_D->query_factor(denomination2);
    if (currency1 == currency2)
      factor = 1.0;
    else
      factor = 1.0 - (exchange_fee / 100.0);
    amount2 = to_int(player->query_amt_currency(currency1) 
		     * rate1 * factor / rate2);
    write("You can get upto "
	  +MONEY_D->denomination_to_string(amount2, denomination2)
	  +".\n");
    input_one_arg("How many "+MONEY_D->query_plural(denomination2)
		  +" do you want? ",
		  (: exchange4b, currency1, denomination2 :) );
  }
  else
    write("Sorry, we don't have any "+denomination2+".\n");
}
   
void exchange4(string denomination1, int amount1, string currency2) {
  string currency1;
  float rate1, rate2, factor;
  float amount2;
  mapping money;
  
  if (MONEY_D->is_currency(currency2)) {
    currency1 = MONEY_D->query_currency(denomination1);
    rate1 = to_float(MONEY_D->query_exchange_rate(currency1)) 
      * MONEY_D->query_factor(denomination1);
    rate2 = to_float(MONEY_D->query_exchange_rate(currency2));
    if (currency1 == currency2)
      factor = 1.0;
    else
      factor = 1.0 - (exchange_fee / 100.0);
    amount2 = amount1 * rate1 * factor / rate2;
    money = MONEY_D->calculate_denominations(amount2, currency2);
    if (sizeof(money)) {
      player->subtract_money(denomination1, amount1);
      foreach (string denomination, int amount in money)
	player->add_money(denomination, amount);
      write("You exchange "
	    +MONEY_D->denomination_to_string(amount1, denomination1)+" into "
	    +MONEY_D->currency_to_string(money, currency2)+".\n");
    }
    else
      write("The lowest denomination of "+currency2+" is worth more than "
	    +MONEY_D->denomination_to_string(amount1, denomination1)+".\n");
  } 
  else
    write("Sorry, we don't have any "+currency2+".\n");
}

void exchange3(string denomination, mixed answer) {
  int amount = to_int(answer);
  if (amount > 0) {
    if (player->query_amt_money(denomination) >= amount)
      input_one_arg("Which currency do you want? ",
		    (: exchange4, denomination, amount :) );	
    else
      write("You don't have "
	    +MONEY_D->denomination_to_string(amount, denomination)+".\n");
  }
}
 
void exchange2(string denomination) {  
  if (denomination != "") {
    if (denomination == deposit_currency
	|| (!MONEY_D->is_denomination(denomination) 
	    && MONEY_D->is_currency(denomination))) {
      printf("You have %s.\n", 
	     MONEY_D->currency_to_string(player->query_money(), denomination));
      input_one_arg("Which denomination do you want? ",
		    (: exchange3b, denomination :) );
    }
    else if (MONEY_D->is_denomination(denomination)) {
      if (player->query_amt_money(denomination) > 0) {
	write("You have "+MONEY_D->denomination_to_string(
                            player->query_amt_money(denomination), 
			    denomination)+".\n");
	input_one_arg("How many "+MONEY_D->query_plural(denomination)+
		      " do you want to exchange? ",
		      (: exchange3, denomination :) );
      } 
      else
	write("You don't have any "+denomination+".\n");
    }
    else
      write("Unknown denomination: "+denomination+".\n");
  }
}
  
void exchange1() {
  input_one_arg("Which currency or denomination do you want to exchange? ", 
		(: exchange2 :) );
}

void deposit3(string denomination, mixed answer) {
  int amount = to_int(answer);
  if (amount > 0) {
    if (player->query_amt_money(denomination) >= amount) {
      player->subtract_money(denomination, amount);
      ACCOUNT_D->deposit(bank_id, player, 
			 amount * MONEY_D->query_factor(denomination));
      printf("You deposit %s.\n",
	     MONEY_D->denomination_to_string(amount, denomination));
      printf("Your new account balance is %s.\n\n",
	     MONEY_D->currency_to_string(
	       ACCOUNT_D->query_account(bank_id, player), deposit_currency));
    } 
    else
      write("You don't have "
	    +MONEY_D->denomination_to_string(amount, denomination)+".\n");
  }
}

void deposit2(string denomination) {
  if (MONEY_D->is_denomination(denomination)) {
    if (MONEY_D->query_currency(denomination) == deposit_currency) {
      input_one_arg("How many "+MONEY_D->query_plural(denomination)+
		    " do you want to deposit? ",
		    (: deposit3, denomination :) );
    }
    else
      write(capitalize(denomination)+" is not a denomination of "
	    +deposit_currency+".\n");
  }
  else
    write("Unknown denomination: "+denomination+".\n");
}

void deposit1() {
  mapping money = player->query_money();
  write("You have "
	+ MONEY_D->currency_to_string(money, deposit_currency)+".\n");
  input_one_arg("Which denomination do you want to deposit? ",
		(: deposit2 :) );
}

void withdraw3(string denomination, mixed answer) {
  int amount = to_int(answer);
  if (amount > 0) {
    if (ACCOUNT_D->query_account(bank_id, player) 
	>= amount * MONEY_D->query_factor(denomination)) {
      ACCOUNT_D->withdraw(bank_id, player, 
			  amount * MONEY_D->query_factor(denomination));
      player->add_money(denomination, amount);
      printf("You withdraw %s.\n", 
	     MONEY_D->denomination_to_string(amount, denomination));
      printf("Your new account balance is %s.\n\n",
             MONEY_D->currency_to_string(
	       ACCOUNT_D->query_account(bank_id,player), deposit_currency));
    } 
    else
      write("You don't have "+answer+" "+denomination
	    +" in your account.\n");
  }
}

void withdraw2(string denomination) {
  if (MONEY_D->is_denomination(denomination)) {
    if (MONEY_D->query_currency(denomination) == deposit_currency) {
      input_one_arg("How many "+MONEY_D->query_plural(denomination)+
		    " do you want to withdraw? ",
		    (: withdraw3, denomination :) );
    }
    else
      write(capitalize(denomination)+" is not a denomination of "
	    +deposit_currency+".\n");
  }	
  else
    write("Unknown denomination: "+denomination+".\n");
}

void withdraw1() {
  printf("Your current account balance is %s.\n\n",
	 MONEY_D->currency_to_string(
	   ACCOUNT_D->query_account(bank_id, player), deposit_currency));
  input_one_arg("Which denomination do you want to withdraw? ",
		(: withdraw2 :) );
}

void quit_menu() {
  modal_pop();
  do_game_command("say Thanks for using "+bank_name+", "
		  +this_body()->short());
  if(was_busy) {
    was_busy--;
    do_game_command("say Next one please");
  }
  player = 0;
}

void mudlib_setup () {
  MENU_ITEM main_seperator = (MENU_ITEM) new_seperator(
    "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
  toplevel = (MENU) new_menu();
  add_menu_item (toplevel, main_seperator);
  add_menu_item (toplevel, new_menu_item("Show the money you have", 
					 (: show_money :), "s"));
  add_menu_item (toplevel, new_menu_item("List exchange rates", 
					 (: show_rates :), "l"));
  add_menu_item (toplevel, new_menu_item("Exchange currencies", 
					 (: exchange1 :), "x"));
  add_menu_item (toplevel, new_menu_item("Deposit money", 
                                          (: deposit1 :), "d"));
  add_menu_item (toplevel, new_menu_item("Withdraw money", 
                                          (: withdraw1 :), "w"));
  add_menu_item (toplevel, new_menu_item("Quit", (:quit_menu:), "q"));
  set_id("accountant");
}

void begin_conversation() {
  if (player) {
    do_game_command("say Hello "+this_body()->short()+", welcome to "
		    +bank_name+". I'm busy at the moment. "
		    "Please wait a little.");
    was_busy++;
  } else {
    player = this_body();
    do_game_command("say Hello "+this_body()->short()+", welcome to "
		    +bank_name+". We will be pleased to exchange your "
		    "money for only a minor fee of "+exchange_fee+"%. "
		    "You can deposit your money in "
		    +MONEY_D->query_plural(deposit_currency)+".\n");
    set_menu_title(toplevel, "Main Menu of "+bank_name);
    init_menu_application(toplevel);
  }
}

mixed direct_talk_to_liv() {
  return 1;
}

mixed direct_talk_with_liv() {
  return 1;
}

void remove() {
  living::remove();
  menu::remove();
}

mapping lpscript_attributes() {
    return ([
	"bank_id" : ({ LPSCRIPT_STRING, "setup", "set_bank_id" }),
	"bank_name" : ({ LPSCRIPT_STRING, "setup", "set_bank_name" }),
	"currency_type" : ({ LPSCRIPT_STRING, "setup", "set_currency_type" }),
        "exchange_fee" : ({ LPSCRIPT_INT, "setup", "set_exchange_fee" }),
    ]);
}

