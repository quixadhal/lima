/* Do not remove the headers from this file! see /USAGE for more info. */

/* replacement for COINS becuase not all money is in coins */

inherit OBJ;
inherit M_GETTABLE;
inherit M_MESSAGES;

private mapping money = ([]);

string long_descr() {
  string descr;
  string denomination;
  int amount;

  foreach (denomination, amount in money) {
    if (descr)
      descr += ", ";
    else
      descr = "A pile of money consisting of ";
    descr += MONEY_D->denomination_to_string(amount, denomination);
  }
  return descr;
}

private int query_amt_money(string type) {
  return money[type];
}

string *query_money() {
  return keys(money);
}

void split_money(int amount, string type) {
  if(money[type] >= amount) {
    this_body()->add_money(type, amount);
    money[type] -= amount;
    this_body()->simple_action("$N $vtake "
			       +MONEY_D->denomination_to_string(amount, type)
			       +".");
    if (money[type] <= 0) {
      map_delete(money,type);
      if (!sizeof(query_money()))
	remove();
    }
  }
  else
    write("There aren't that many money of that type here.\n");
}

mixed get(string amount_str, string type) {
  string denomination;
  int amount;
  string *types;
  
  if(!amount_str) {
    foreach (denomination, amount in money) {
      this_body()->add_money(denomination, amount);
    }
    this_body()->simple_action("$N $vtake the pile of money.");
    remove();
    return "";
  }
  else {
    if(type == "coins" || type == "coin") {
      types = query_money();
      if (sizeof(types) == 1) {
	amount = to_int(amount_str);
	split_money(amount, types[0]);
      }
      else
	write("Which type of money do you want to get?\n");
    }
    else if(!money[type])
      write("There are no money of that type here.\n");
    else {
      if (amount_str == "all")
        split_money(money[type], type);
      else
	split_money(to_int(amount_str), type);
    }
  }
}

void merge_money(int amount, string type) {
  this_object()->add_id(type);
  money[type] += amount;
}

void setup(int amount, string type) {
  amount = to_int(amount);
  money[type] = amount;
  set_id("money", type, "coins", "pile", "pile of money");
  add_adj(type);
  set_in_room_desc("A pile of money");
  set_proper_name("money");
  set_long( (: long_descr :) );
  set_gettable(1);
  set_plural(1);
}

