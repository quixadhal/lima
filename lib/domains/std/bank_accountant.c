/* Do not remove the headers from this file! see /USAGE for more info. */

/* $Id: bank_accountant.c,v 1.5 1998/02/04 23:35:16 monica Exp $ */

inherit ACCOUNTANT;

void setup() {
  set_name("Samuel");
  add_id("accountant", "sam");
  set_gender(1);
  set_proper_name("Samuel the Bank Accountant");
  set_in_room_desc("Samuel the Bank Accountant stands behind the counter.");
  set_long("Samuel is a boring looking balding man. Perfectly clothed of "
	   "course.");
  set_bank_id("zet");
  set_bank_name("First National Bank of Zetenet");
  set_currency_type("dollar");
  set_exchange_fee(5);
}
