/* Do not remove the headers from this file! see /USAGE for more info. */




#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;
inherit M_MESSAGES;

private mapping coins = ([]);

int long_amount;
string long_type;

string long_descr()
{
    string *types;
    string descr;
    int i;

    types = this_object()->query_coins();
    i = sizeof(types);
    if (i == 1)
    {

add_id(long_type);
	descr = "A pile of ";

	descr += coins[long_type];
	descr += " ";
	descr += types[0];
  descr += " coins.\n";
    }
    else
    {
descr = "A pile of various types of coins including: \n ";
   while (i--)
   {
descr += "          ";
     descr += types[i];
descr += "\n";
   }
    }
    return descr;
}

private int query_amt_coins(string type)
{
    return coins[type];
}

int *query_coins()
{
    return keys(coins);
}


mixed get(string amount, string type)
{
    string *types;
    int coin_value;
    int int_amount;
    int i;
    if(!amount)
    {
	types = this_object()->query_coins();
	i = sizeof(types);

	if(i == 1)
	{
	    coin_value = this_object()->query_amt_coins(types[0]);
	    this_body()->add_money(types[0], coin_value);
	    remove();
	    return "You take the pile of coins.\n";
	}
	else
	{
	    while(i--)
	    {
		coin_value = this_object()->query_amt_coins(types[i]);
		this_body()->add_money(types[i], coin_value);
	    }
	    remove();
	    return "You take the pile of coins.\n";
	}
    }

    else
    if(type == "coins" || type == "coin")
    {
	types = this_object()->query_coins();
	i = sizeof(types);
	if (i == 1)
	{
	    int_amount = to_int(amount);
	    this_object()->split_coins(types[0], int_amount);
	}
	else
	{
	    write("Which type of coins do you wnat to get?\n");
	}
    }
    else
    {
	if(!coins[type])
	{
	    write("There are no coins of that type here.\n");
	}
	else
	{
	    int_amount = to_int(amount);
	    this_object()->split_coins(type,int_amount);
	}
    }
}

void split_coins(string type, int amount)
{
    string *types;
    int i;


    if(coins[type] >= amount)
    {
	this_body()->add_money(type,amount);
	coins[type] -= amount;
	this_body()->simple_action("$N $vtake some coins.\n");
	if (coins[type] == 0)
	{
	    map_delete(coins,type);
	}
	types = this_object()->query_coins();
	i = sizeof(types);
	if (!i)
	{
	    remove();
	}
    }
    else
    {
	write("There aren't that many coins of that type here.\n");
    }
}

void merge_coins(int amount, string type)
{
this_object()->add_id(type);
    if (!coins[type])
    {
	coins[type] = amount;
    }
    else
    {
	coins[type] += amount;
    }
}


void setup(int amount, string type)
{
    long_type = type;
    coins[type] = to_int(amount);

    set_id("coins");
    add_adj(type);
    set_in_room_desc("A pile of coins");
    add_id(   "coin", "pile", "pile of coins" );
    set_long( (: long_descr :) );

    set_gettable(1);
}
