/* Do not remove the headers from this file! see /USAGE for more info. */

/* rewritten 7-Mar-98 by MonicaS */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "currency";
}

nomask string module_key() {
    return "c";
}

private nomask void list_currencies() {
  string *types = MONEY_D->query_currency_types();

  if ( sizeof(types) == 0 ) {
    write("No currencies are defined.\n");
    return;
  }
  printf("%-12s%-12s%12s\n%*'-'s\n", "Currency", "Plural", "Rate (Value)", 36, "");
  foreach ( string type in types ) {
    printf("%-12s%-12s%12d\n", capitalize(type), 
	   capitalize(MONEY_D->query_plural(type)),
	   MONEY_D->query_exchange_rate(type));
  }
}

private nomask void add_currency(string type, string plural) {
  MONEY_D->add_currency(type, plural);
  printf("Currency type \"%s\" has been added with plural \"%s\"\n", 
	 type, plural);
}

private nomask void remove_currency(string type) {
  MONEY_D->remove_currency(type);
  printf("Currency type \"%s\" has been deleted.\n", type);
}

private nomask void set_exchange_rate(string type, string rate_str) {
  int rate = to_int(rate_str);
  MONEY_D->set_exchange_rate(type, rate);
  if (rate < 1)
    printf("The exchange rate has to be at least 1\n");
  else
    printf("Exchange rate for \"%s\" set to %d.\n", type, rate);
}

private nomask void list_denomination(string type) {
  if (!MONEY_D->query_denominations(type)) {
    write("illegal type: "+type+".\n");
    return;
  }
  write("Currency "+capitalize(type) + ":\n");
  printf("   Name        Plural     Exchange rate factor  Resulting Value\n"
	 "   ------------------------------------------------------------\n");
  foreach (string name in MONEY_D->query_denominations(type)) {
    printf("   %-12s%-12s  %12.4f      %12d\n", name, 
	   MONEY_D->query_plural(name), MONEY_D->query_factor(name),
	   to_int(MONEY_D->query_factor(name) 
		  * to_float(MONEY_D->query_exchange_rate(type))));
  }
}

private nomask void add_denomination2(string type, string name, string plural,
				     string factor_str) {
  float factor = to_float(factor_str);
  if (factor <= 0) {
    write("illegal factor: "+factor_str+".\n");
    return;
  }
  MONEY_D->add_denomination(type, name, plural, factor);
}

private nomask void add_denomination1(string type, string name) {
  input_two_args("Plural of denomination? ", 
		 "Exchange rate factor? ", 
		 (: add_denomination2, type, name :) );
}

private nomask void remove_denomination(string denomination) {
  string type;
  
  denomination = lower_case(trim_spaces(denomination));
  if (!MONEY_D->is_denomination(denomination)) {
    write("Unknown denomination: "+denomination+".\n");
    return;
  }
  type = MONEY_D->query_currency(denomination);
  MONEY_D->remove_denomination(denomination);
  printf("Removed denomination %s from currency %s.\n", denomination, type);
}

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "c",
	    desc : "list currencies",
	    action : (: list_currencies :)),
	new(class command_info,
	    key : "A",
	    proto : "[type] [plural name]",
	    desc : "add a currency",
	    who : "[Mudlib:daemons]",
	    priv : "Mudlib:daemons",
	    args : ({ "What type? ", "What plural name? " }),
	    action : (: add_currency :)),
        new(class command_info,
	    key : "R",
	    proto : "[type]",
	    desc : "remove a currency",
	    who : "[Mudlib:daemons]",
	    priv : "Mudlib:daemons",
	    args : ({ "What type? " }),
	    action : (: remove_currency :)),	    
	new(class command_info,
	    key : "e",
	    proto : "[type] [rate]",
	    desc : "set exchange rate",
	    who : "[Mudlib:daemons]",
	    priv : "Mudlib:daemons",
	    args : ({ "What type? ", "What rate? " }),
	    action : (: set_exchange_rate :)),
	new(class command_info,
	    key : "d",
	    proto : "[type]",
	    desc : "list denominations",
	    args : ({ "What type? "}),
	    action : (: list_denomination :)),
	new(class command_info,
	    key : "a",
	    proto : "[type] [denomination]",
	    desc : "add a denomination to a currency",
	    priv : "Mudlib:daemons",
	    who : "[Mudlib:daemons]",
	    args : ({ "What type? ", "Add which denomination? "}),
	    action : (: add_denomination1 :)),
	new(class command_info,
	    key : "",
	    desc : "(creates currency type if needed)"),
	new(class command_info,
	    key : "r",
	    proto : "[denomination]",
	    desc : "remove a denomination",
	    priv : "Mudlib:daemons",
	    who : "[Mudlib:daemons]",
	    args : ({ "Remove which denomination? "}),
	    action : (: remove_denomination :)),
    });
}



