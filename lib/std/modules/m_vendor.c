/* Do not remove the headers from this file! see /USAGE for more info. */
#include <mudlib.h>
#include <move.h>

inherit M_SAVE;

string short();
string query_subjective();

private mixed for_sale;
private mixed will_buy;
private mixed currency_type = "gold";
//private string *stored_items = ({ ({ "0", "dull sword", "/wiz/zifnab/sword.c", 5000, ({ "sword", "knife" }) }) });
private string *stored_items = ({});

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
    //if(!member_array(type, MONEY_D->query_valid_currencies() ))
    //error("Not a valid currency");
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

mixed direct_ask_liv_about_wrd(object liv, string item)
{
    return 1;
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

    int object_cost;
    string item_state;
    string item_name;
    string name_of_file;
    string *object_ids;
    if (ob->move(this_object()) != MOVE_OK)
	write("You can't seem to give " + ob->the_short() + " to " + short() +
	  ".\n");
    else {
	int exchange_rate = MONEY_D->query_exchange_rate(currency_type);
	int object_value;

	object_value = ob->query_value() * exchange_rate;
	object_cost = object_value * 1.5;
	this_body()->add_money(currency_type, object_value);
	item_state = save_to_string(1);
	item_name = ob->short();
  object_ids = ob->query_id();
  object_ids += ({ ob->plural_short() });
	stored_items += ({ ({ item_state }) + ({ item_name }) + ({ object_cost }) + ({object_ids }) });
	this_body()->my_action("$N $vsell a $o for "+ object_value +" "+
	  currency_type +".\n", ob);
	this_body()->other_action("$N $vsell a $o.\n", ob);
	ob->remove();

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

//:FUNCTION query_items()
//gets called from the verb ask and the rule ask obj about str
//this function shows the players what items the shopkeeper has
void query_items(string item)
{

    int i;
    int m;
    int j;
    string *stored_items_ids;
    string *items_available = ({});

    i = sizeof(stored_items);
if(!i)
{
   this_body()->my_action("The shopkeeper says, \"I currently have no items in inventory.\"\n");
}
else
{
    while(i--)
    {
	if (item == "all")
	{
	    items_available += ({ ({ stored_items[i][1] }) + ({ stored_items[i][2] }) });
	}
	else
	{
	    stored_items_ids = ({ stored_items[i][3] });
	    if(member_array(item, stored_items_ids[0]) != -1)
	    {
		items_available += ({ ({ stored_items[i][1] }) + ({ stored_items[i][2] }) });
	    }
	}
    }
    j = sizeof(items_available);
    if (j)
    {
	this_body()->my_action("The shopkeeper says, \"I have the following items that match your request.\"\n");
	for( m=0; m < j; m++)
	{
	    printf("%10d%20s %30d\n", m + 1 ,items_available[m][0],items_available[m][1]);
	}
    }
    else
    {
	this_body()->my_action("The shopkeeper says, \"I have no items that match your request.\"\n");
    }
}
}

//:FUNCTION sell_stored_objects()
//Gets called from the buy verb with the string from obj rule.
//This is the way the players can buy objects from the shopkeeper
//that he has stored away.
void sell_stored_objects(string item, int number)
{
    int i;
int j = 0;
    int m = 0;
    string *for_sale_ids;
    object ob;

    i = sizeof(stored_items);


    if (!number)
    {
	number = 1;
    }
    if (i)
    {
while (i--)
	{
	    for_sale_ids = ({ stored_items[i][3] });
	    if(member_array(item, for_sale_ids[0]) != -1)
	    {
		j += 1;
		if (j == number)
		{
		    this_body()->simple_action("$N $vbuy a "+ item +" from the shopkeeper.\n");
		    load_from_string(stored_items[i][0],1);

		    if (ob=present(item, this_object()))
			ob->move(this_body());
		    stored_items -= ({ stored_items[i] });
		    break;
		}
	    }
	}
    }
}

mixed direct_buy_str_from_liv(string str, object liv)
{
    return 1;
}
