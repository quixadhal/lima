/* Do not remove the headers from this file! see /USAGE for more info. */
/* $Id: account_d.c,v 1.1 1998/01/27 23:20:44 monica Exp $ */

inherit M_DAEMON_DATA;

private mapping accounts = ([ ]);

float query_account(string bank, object player) {
  if (accounts[bank])
    return accounts[bank][player->query_link()->query_userid()];
  else
    return 0;
}

void deposit(string bank, object player, float amount) {
  if (!accounts[bank])
    accounts[bank] = ([ ]);
  accounts[bank][player->query_userid()] =
    to_float(accounts[bank][player->query_userid()]) + amount;
  save_me();
}

void withdraw(string bank, object player, float amount) {
  deposit(bank, player, -amount);
}

void set_account(string bank, object player, float amount) {
  if (!accounts[bank])
    accounts[bank] = ([ ]);
  accounts[bank][player->query_userid()] = amount;
  save_me();
}

void clean_up() {
  destruct(this_object());
}
