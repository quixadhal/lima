/* Do not remove the headers from this file! see /USAGE for more info. */

//   
//  This is a simple money implementation
//
//    written by zifnab@lima bean 10/95
//
//  please comment away and feel free to modify anything you want
//
//  also please be kind this is my first attempt at anything like
//  this :).  please note that there are no checks of any kind in here yet
//  to see if the type of currency is valid or not.
//
//  cleaned up 10-Feb-98 by MonicaS

private mapping money = ([]);

//:FUNCTION query_amt_money
//This is the functin to call to query the amount of a certain type
//of currency you have.
int query_amt_money(string type)
{
    type = MONEY_D->singular_name(type);
    return money[type];
}

//:FUNCTION query_amt_currency
//This is the functin to call to query the sum of the types of a
//currency you have.
float query_amt_currency(string currency)
{
    float amount = 0.0;
    currency = MONEY_D->singular_name(currency);
    foreach (string type in MONEY_D->query_denominations(currency)) {
        amount += to_float(money[type]) * MONEY_D->query_factor(type);
    }
    return amount;
}

//:FUNCTION add_money
//This is the function to call to add money to a person 
void add_money(string type, int amount)
{
    type = MONEY_D->singular_name(type);
    money[type] += amount;

    if(money[type] <= 0)
	map_delete(money, type);
}

//:FUNCTION subtract_money
//This is the function to call to substract money from a person 
void subtract_money(string type, int amount)
{
    add_money(type, -amount);
}


//:FUNCTION query_currencies
//This function will return the current "types" of money you have
string *query_currencies()
{
    if(!money)
	return ({});

    return keys(money);
}

//:FUNCTION query_money
//This function will return the complete money mapping
mapping query_money()
{
    return money;
}
