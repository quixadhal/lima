/* Do not remove the headers from this file! see /USAGE for more info. */
#include <move.h>

string short();
string query_subjective();

private mixed for_sale;
private mixed will_buy;
private mixed currency_type = "gold";

//:FUNCTION set_for_sale
//Set the array of object names which this living object is willing to sell.
//set_for_sale(1) means everything is for sale.  set_for_sale(0) means nothing
//is.  A function pointer is legal here.
void set_for_sale(mixed f) {
    for_sale = f;
}

//:FUNCTION set_will_buy
//Set the array of object names which this living object is willing to buy.
//set_will_buy(1) means it will buy anything.  set_will_buy(0) means it wont
//by anything.  A function pointer is legal here.
void set_will_buy(mixed f) {
    will_buy = f;
}

//:FUNCTION set_currency_type
//Sets the type of currency the vendor will buy/sell in
mixed set_currency_type(string type)
{
    if(!member_array(type, MONEY_D->query_valid_currencies() ))
	error("Not a valid currency");
    currency_type = type;
}

mixed query_currency_type()
{
    return currency_type;
}

mixed indirect_buy_obj_from_liv(object ob, object liv) {
    mixed tmp = evaluate(for_sale);

    if (intp(tmp)) return tmp;
    if (!pointerp(tmp)) return 0;

    foreach (string name in tmp) {
	if (ob->id(name)) return 1;
    }
    return 0;
}

mixed indirect_sell_obj_to_liv(object ob, object liv) {
    mixed tmp = evaluate(will_buy);

    if (intp(tmp)) return tmp;
    if (!pointerp(tmp)) return 0;

    foreach (string name in tmp) {
	if (ob->id(name)) return 1;
    }
    return 0;
}

void buy_object(object ob) {
    if (ob->move(this_object()) != MOVE_OK)
	write("You can't seem to give " + ob->the_short() + " to " + short() + ".\n");
    else {
	int exchange_rate = MONEY_D->query_exchange_rate(currency_type);
	int object_value;

	object_value = ob->query_value() * exchange_rate;
	this_body()->add_money(currency_type, object_value);
	this_body()->my_action("$N $vsell a $o for "+ object_value +" "+ currency_type +".\n", ob);
	this_body()->other_action("$N $vsell a $o.\n", ob);

    }
}

void sell_object(object ob) {
    int exchange_rate = MONEY_D->query_exchange_rate(currency_type);
    int object_cost;

    /* the vendor sells things for 1.5 times their value */
    object_cost = ((ob->query_value() * exchange_rate) * 3) / 2;
    if(object_cost > this_body()->query_amt_money(currency_type))
    {
	write( short() +" laughs in your face saying, that costs  "+ object_cost +" "+ currency_type +", which you don't have!\n");
	return 0;
    }
    if (ob->move(this_body()) != MOVE_OK)
	write(capitalize(query_subjective()) + " can't seem to give " + ob->the_short() + " to you.\n");
    else {
	this_body()->subtract_money(currency_type, object_cost);
	this_body()->my_action("$N $vbuy a $o for "+ object_cost +" "+ currency_type +".\n",ob);

	this_body()->other_action("$N $vbuy a $o.\n",ob);
    }
}
