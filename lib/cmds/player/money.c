/* Do not remove the headers from this file! see /USAGE for more info. */
/* $Id: money.c,v 1.4 1998/02/11 02:08:47 monica Exp $ */

//:PLAYERCOMMAND
//$$ see: inventory
//USAGE money
//
//Displays how much money you are carrying

inherit CMD;

private void main() {
  string array currencies = ({ });
  mapping money = this_body()->query_money();

  if ( sizeof(money) == 0 )
  {
    write("You dig around in your pockets and find only lint!\n");
  } else {
    write("You have the following types of money:\n");
    foreach (string type in keys(money))
    {
      if (MONEY_D->is_denomination(type))
	currencies |= ({ MONEY_D->query_currency(type) });
      else
	this_body()->subtract_money(type, money[type]);
    }
    foreach (string currency in currencies)
      printf("   %-12s%s\n", capitalize(currency)+":",
	     MONEY_D->currency_to_string(money, currency));
  }
}