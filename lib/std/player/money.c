/* Do not remove the headers from this file! see /USAGE for more info. */

//   
//   This is a simple money implementation
//
//    written by zifnab@lima bean 10/95
//
//  please comment away and feel free to modify anything you want
//
//  also please be kind this is my first attempt at anything like thi
//  this :).  please note that there are no checks of any kind in here yet
// to se if the type of currency is valid or not
//

#include <mudlib.h>


private   mapping money;


//  This is the functin to call to query the amount of a certain type
//  of currency you have.
//
int query_amt_money(string type)
{
    if(!money)
	return 0;

    if(!money[type])
	return 0;

    return money[type];
}

//
//  This is the function to call to add money to a person 
//   
void add_money(string type, int amount)
{
    if(!money)
	money = ([]);

    if(!type || !amount)
	return;

    if(!money[type])
	money[type] = amount;
    else
	money[type] += amount;

    if(money[type] < 1)
	map_delete(money, type);
}

void subtract_money(string type, int amount)
{
    add_money(type, -amount);
}


//   This function will return the current "types" of money you have
//
int *query_currencies()
{
    if(!money)
	return ({});

    return keys(money);
}

int drop_coins(int amount, string type)
{
    object ob;



    if(query_amt_money(type) < amount)
    {
	write("You don't have "+ amount +" "+ type +" coins.\n");
	return 0;
    }
    else
    {
	subtract_money(type,amount);
	this_body()->simple_action("$N $vdrop "+ amount +" "+ type +" coins.\n");
	if(ob = present("coins"))
	{
	    ob->merge_coins(amount,type);
	}
	else
	{
	    new("/std/coins", amount,type)->move(environment(this_body())); 
	    return 1;
	}
    }
}
