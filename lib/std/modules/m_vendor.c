/* Do not remove the headers from this file! see /USAGE for more info. */
/* $Id: m_vendor.c,v 1.5 1998/03/05 01:54:30 monica Exp $ */

/* Vendor module
 *
 * Zifnab wrote it at some point the first time but didn't put his name in here
 * Darklord went nuts and redid it 15-Feb-97
 * Bunch of changes and additions 25-Apr-97 by Vette
 * Mostly rewritten 6-Feb-98 by MonicaS
 */

//:MODULE
//m_vendor is used to create vendor objects that buy and sell stuff
//they work as traditional shopkeepers and also bartenders
//See: /domains/std/shopkeeper.c

/* TODO
compatibility of buying and selling vehicles
problem lies in ob->move(this_object()), and vehicles are too large.
*/

// Shall the vendor sell objects which were in the store-room befor the start?
// If you define this vendors may not share a store-room.
#ifndef VENDOR_SHARE_STORAGE
#define SELL_OLD_OBJECTS
#endif

// the minimal value of sold items (for objects with query_value() == 0)
#ifdef VENDOR_MIN_VALUE
#define MIN_VALUE VENDOR_MIN_VALUE
#else
#define MIN_VALUE 70
#endif

// Value of what the objects value is multiplied by to achieve its cost
#ifdef VENDOR_COST_MULT
#define COST_MULT VENDOR_COST_MULT
#else
#define COST_MULT 1.5
#endif

#include <move.h>

class item {
  string short;
  string long;
  string file;
  int value;
  string array ids;
  int amount;
  object array objects;
}

string short();
string query_subjective();
int check_uniqueness(object ob);

private mixed for_sale;
private mixed will_buy;
private mixed currency_type = "gold";
private mapping stored_items = ([ ]);
private string unique_inv = "";
private int all_unique = 0;
private int max_item_number = 0;
private float cost_mult = COST_MULT; 

//:FUNCTION set_cost_multiplicator
float set_cost_multiplicator(float m) {
  cost_mult = m;
}

//:FUNCTION selling_cost
//multiply the objects value with cost_mult to get the cost for selling
float selling_cost(float cost) {
  if (cost < MIN_VALUE)
    return MIN_VALUE * cost_mult;
  else
    return cost * cost_mult;
}

//:FUNCTION buying_cost
//override if you want a different way to determine cost
float buying_cost(float cost) {
  return cost;
}

private class item init_item(object ob) {
  class item item;
  item = new(class item, short: ob->short(), long: ob->long(),
	     ids: ob->query_id() + ({ ob->short(), ob->plural_short() }), 
	     value: ob->query_value(), amount: 1);
  if (item->long[<1] != '\n')
    item->long += "\n";
  return item;
}

//:FUNCTION add_sell
//enables you to add items to the vendors stored_item's mapping
void add_sell(string file, int amt) {
  object ob;
  class item item;
  
  if(!amt) amt = 1;
  if(file) {
    if (ob = new (file)) {
      item = init_item(ob);
      item->amount = amt;
      item->file = base_name(ob);
      stored_items[++max_item_number] = item;
      destruct(ob);
    }
    else
      error("No such item: " + file + "\n");
  }
}

//:FUNCTION set_sell
// with a mapping you can set many items into the vendor's to sell list
void set_sell(mapping items) {
  string item;
  int amt;

  foreach (item, amt in items)
    add_sell(item, amt);
}

//:FUNCTION add_sell_object
// adds a unique item to the vendor's stored_items mapping
void add_sell_object(object ob) {
  class item item;
  
  foreach (item in values(stored_items)) {
    if (item->objects && compare_objects(item->objects[0], ob)) {
      item->objects += ({ ob });
      item->amount++;
      return;
    }
  }
  item = init_item(ob);
  item->objects = ({ ob });
  stored_items[++max_item_number] = item;
}

//:FUNCTION set_for_sale
//Set the array of object names which this living object is willing to sell.
//set_for_sale(1) means everything is for sale.  set_for_sale(0) means nothing
//is.  If a function is passed it will get the object to sell as argument.
//If a single string is returned it will be used as error message.
void set_for_sale(mixed x) {
  for_sale = x;
}

mixed query_for_sale() {
  return for_sale;
}

//:FUNCTION set_will_buy
//Set the array of object names which this living object is willing to buy.
//set_will_buy(1) means it will buy anything.  set_will_buy(0) means it wont
//by anything.  If a function is passed it will get the object to buy as
//argument. If a single string is returned it will be used as error message.
void set_will_buy(mixed x) {
  will_buy = x;
}

mixed query_will_buy() {
  return will_buy;
}

//:FUNCTION set_currency_type
//Sets the type of currency the vendor will buy/sell in
mixed set_currency_type(string type) {
  if(!MONEY_D->is_currency(type))
    catch("Invalid currency set in shopkeeper!\n");
  currency_type = type;
}

//:FUNCTION query_currency_type
//Queries the type of currency the vendor will buy/sell in
mixed query_currency_type()
{
  return currency_type;
}

// do NOT overload any of the direct or indirect rules. They are not called 
// if this is the only vendor and no vender is specified in the call.
// Use set_will_buy() and set_for_sale() instead.

mixed direct_ask_liv_about_wrd(object liv, string item) {
  return 1;
}

mixed direct_ask_liv_about_str(object liv, string item) {
  return 1;
}

mixed direct_list_from_liv(object liv) {
  return 1;
}

mixed direct_list_str_from_liv(string item, object liv) {
  return 1;
}

mixed direct_buy_str_from_liv(string item, object liv) {
  return 1;
}

mixed indirect_sell_obj_to_liv(object ob, object liv) {
  return 1;
}

int test_buy(object ob) {
  mixed result;
  
  if (!will_buy) {
     write(capitalize(short())+" doesn't buy anything.\n");
     return 0;
  }
  result = evaluate(will_buy, ob);
  if (result == 1)
    return 1;
  if (stringp(result)) {
    if (result[<1] != '\n') result += "\n";
    write(result);
    return 0;
  }
  if (arrayp(result)) {
    foreach (string name in result) {
      if (ob->id(name)) return 1;
    }
  }
  write(capitalize(short())+" doesn't want to buy "+ob->the_short()+".\n");
  return 0;
}

//FUNCTION: buy_object
//gets called from the verb sell. Addes bought object to the list of
//stored_items depending on check_uniqueness()
void buy_object(object ob) {
  float cost;
  mixed item;
  string file;
  mapping money;
  int exchange_rate = MONEY_D->query_exchange_rate(currency_type);
  
  if (!test_buy(ob)) 
    return;
  cost = buying_cost(to_float(ob->query_value())) / to_float(exchange_rate);
  if (cost < 0.01) {
    write("You wouldn't get any "+currency_type+" for "+ob->the_short()+".\n");
    return;
  }
  if (ob->move(this_object()) != MOVE_OK) {
    write("You can't seem to give "+ob->the_short()+" to "+short()+".\n");
    return;
  }
  money = MONEY_D->calculate_denominations(cost, currency_type);
  foreach (string type, int amount in money) {
    this_body()->add_money(type, amount);
  }
  this_body()->my_action("$N $vsell a $o for " 
			 + MONEY_D->currency_to_string(money, currency_type)
			 + ".\n", ob);
  this_body()->other_action("$N $vsell a $o.\n", ob);
   
  switch (check_uniqueness(ob)) {
  case 0: /* object is not unique, so just keep the filename. */
    file = base_name(ob);
    destruct(ob);
    foreach (item in values(stored_items)) {
      if (item->file && item->file == file) {
	if (item->amount != -1)
	  item->amount++;
	return;
      }
    }
    add_sell(file, 1);
    break;
  case 1: /* object is unique */
    add_sell_object(ob);
    ob->move(load_object(unique_inv));
    break;
  case 2: /* object is destroyable(), so don't sell it */
    destruct(ob);
    break;
  }
}

//:FUNCTION query_items
//gets called from the verb ask and the rule ask obj about str
//The player commands buy and list use it too.
//This function shows the players what items the shopkeeper has.
//If flag is set the it will show the long() too
int query_items(string item, int flag) {
  int *keys = ({});
  int key, num;
  string cost;
  float exchange_rate = to_float(MONEY_D->query_exchange_rate(currency_type));

  if(sizeof(stored_items) == 0 || !for_sale) {
    write("This shop has nothing to sell.\n");
    return 0;
  }
  if (item == "all") {
    keys = keys(stored_items);
  } else {
    foreach (key in keys(stored_items)) {
      if (member_array(item, stored_items[key]->ids) != -1) {
	keys += ({ key });
      }
    }
  }
  if (sizeof(keys) == 0) {
      write("Nothing in this shop matches that!\n");
      return 0;
  }
  keys = sort_array(keys, 1);
  printf("%|6s %|8s %-24s %s\n","List #", "Amount", "Name/id",
		capitalize(currency_type));
  foreach (key in keys) {
    cost = MONEY_D->currency_to_string(
             selling_cost(to_float(stored_items[key]->value)) / exchange_rate, 
	     currency_type);
    num = stored_items[key]->amount;
    if (num != -1)
      printf("%|6d %|8d %-24s %s\n", key, num,
	     stored_items[key]->short, cost);
    else 
      printf("%|6d %|8s %-24s %s\n", key, "Numerous", 
	     stored_items[key]->short, cost);
    if (flag)
      write(stored_items[key]->long);
  }
  return 1;
}

int test_sell(object ob) {
  // for_sale == 0 is tested in sell_stored_objects() already
  mixed result = evaluate(for_sale, ob);
  if (result == 1)
    return 1;
  if (stringp(result)) {
    if (result[<1] != '\n') result += "\n";
    write(result);
    return 0;
  }
  if (arrayp(result)) {
    foreach (string name in result) {
      if (ob->id(name)) return 1;
    }
  }
  write(capitalize(short())+" doesn't want to sell "+ob->the_short()+".\n");
  return 0;
}

protected int sell_object(object ob) {
  float exchange_rate = to_float(MONEY_D->query_exchange_rate(currency_type));
  float cost; 
  mapping array money;
  object cont;

  if (!test_sell(ob)) 
    return 0;
  cost = selling_cost(to_float(ob->query_value())) / exchange_rate;
  if (cost > this_body()->query_amt_currency(currency_type)) {
    printf("Sorry, that costs %s, which you don't have!\n",
	   MONEY_D->currency_to_string(cost, currency_type));
    return 0;
    }
  if (ob->query_default_container())
  {
    cont = new(ob->query_default_container());
    ob->move(cont);
    ob = cont;
  }
  if (ob->move(this_body()) != MOVE_OK) {
    write(capitalize(query_subjective()) + " can't seem to give " 
	  + ob->the_short() + " to you.\n");
	  if(ob->move(environment(this_body())) == MOVE_OK)
	  {
      write(capitalize(query_subjective()) +
          " leaves it on the floor instead.\n");
      return 1;
	  }
    return 0;
  }
  money = MONEY_D->handle_subtract_money(this_body(), cost, currency_type);
  this_body()->my_action("$N $vbuy a $o for " 
			 +MONEY_D->currency_to_string(cost, currency_type)
			 +". You give "
			 +MONEY_D->currency_to_string(money[0], currency_type)
			 +" to $o1"
			 +(sizeof(money[1]) ? " and get "
			   +MONEY_D->currency_to_string(money[1],currency_type)
			   +" as change" : "")
			 +".\n", ob, this_object());
  this_body()->other_action("$N $vbuy a $o.\n", ob);
  return 1;
}

private void sell_items(int key, int amount) {
  int i;
  class item item;
  object ob;

  item = stored_items[key];
  if (item->amount != -1 && item->amount < amount)
    amount = item->amount;

  for (i = 1; i <= amount; i++) {
    if (item->objects)
      ob = item->objects[0];
    else
      ob = new(item->file);
    if (sell_object(ob)) {
      if (item->objects)
	item->objects -= ({ ob });
      if (item->amount != -1) {
	item->amount--;
	if (item->amount <= 0)
	  map_delete(stored_items, key);
      }
    }
    else
      return;
  }
}

//:FUNCTION sell_stored_objects
//Gets called from the buy verb with the string from obj rule.
//This is the way the players can buy objects from the shopkeeper
//that he has stored away.
void sell_stored_objects(string item, int number, int amount) {
  if (!for_sale) {
    write("This shop doesn't sell anything.\n");
    return;
  }
  if (amount == 0) amount = 1;
  if (number) {
    if (!stored_items[number]) {
      write("There is no item #"+number+" available.\n");
      return;
    }
    if (item == "all")
      sell_items(number, amount);
    else {
      if (member_array(item, stored_items[number]->ids) != -1)
	sell_items(number, amount);
      else
	write("There is no '"+item+"' at #"+number+".\n");
    }
  }
  else {
    foreach (int key in keys(stored_items)) {
      if (member_array(item, stored_items[key]->ids) != -1) {
	sell_items(key, amount);
	return;
      }
    }
    write("There is no '"+item+"' in the list.\n");
  }
}

// for debugging only
mapping query_stored() {
  return stored_items;
}

//:FUNCTION set_unique_inventory
//void set_unique_inventory(string str)
//This function determines if the vendor should hold onto what he
//buys instead of desting it and replacing it with an original.  For
//Example without unique set if you sell a sword to the vendor, no
//matter what the condition is, it will be dested and replaced with
//a new one.  Some muds would prefer the old way of what you sell is
//what you buy.  The unique inventory is set by sending the room
//where the inventory is kept.
//ex: set_unique_inventory("/domains/std/rooms/storage");
//NOTE: only armor, weaps, vehicles are uniqued
//Unless the object has a is_unique() { return 1; } function in it
//See set_all_unique to unique everything
void set_unique_inventory(string str)
{
  unique_inv = str;
#ifdef SELL_OLD_OBJECTS
  foreach (object ob in all_inventory(load_object(str)))
    add_sell_object(ob);
#endif
}

string query_unique_inventory()
{
  return unique_inv;
}

//:FUNCTION set_all_unique
//Sets ALL objects to be uniqued.  Only works when set_unique_inventory()
//is used.
void set_all_unique(int i)
{
  all_unique = i;
}

int query_all_unique()
{
  return all_unique;
}

int query_vendor()
{
  return (for_sale || will_buy);
}

//:FUNCTION check_uniqueness
//This fuction test if an object should be destroyed or saved when bought,
//depending on destroyable(), set_all_unique() and is_unique().
int check_uniqueness(object ob)
{
  /* If the object is supposed to be removed from the game upon selling */
  /* ie. quest objects */
//:FUNCTION destroyable
//place int destroyable() { return 1; } if you do not wish your object
//to be purchaseable from a vendor.  Players may sell it to receive its
//value, but it will then be destroyed.  Mostly used for quest objects
  if (ob->destroyable()) return 2;

  /* If we dont allow uniqueness at all, atuo return 0 */
  if (!unique_inv || unique_inv == "") return 0;

  /* If we want everything to be unique, return 1 */
  if (all_unique) return 1;

  /* If the object has been coded to be unique, return 1 */
//:FUNCTION is_unique
//put int is_unique() { return 1; } in your object if you wish it
//to be kept in a storeroom.  This is most likely because your
//object would have variable changes in it.  For example, weapons
//can have their wc reduced by wear and you'd rather someone not be
//able to purchase a brand new sword after selling a broken one.
  if (ob->is_unique()) return 1;

  if (ob->is_armor()) return 1;
  if (ob->is_weapon()) return 1;
  if (ob->is_vehicle()) return 1;
  return 0;
}

mapping lpscript_attributes() {
    return ([
	"currency_type" : ({ LPSCRIPT_STRING, "setup", "set_currency_type" }),
        "for_sale" : ({ LPSCRIPT_BOOLEAN, "setup", "set_for_sale" }),
        "will_buy" : ({ LPSCRIPT_BOOLEAN, "setup", "set_will_buy" }),
	"sell" : ({ LPSCRIPT_INT_MAPPING, "setup", "set_sell" }),
    ]);
}
