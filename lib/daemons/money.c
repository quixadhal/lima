/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** money_d.c -- money daemon
**
** This daemon manages the legal types of money within the game and their
** exchange rates.
**
** Objects have an inherent "value".  This is then translated into a
** particular currency via that currency's value -> actual exchange rate.
**
** 951205, Zifnab: created
** 951213, Deathblade: installed as money_d.c
*/

//### this daemon needs interfaces with admtool to manage the types and
//### the exchange rates.
//### Note: if these should only be adjustable by an admin, then this
//### daemon must move to /secure/daemons to prevent tampering with any
//### security restrictions that are added.


//### these should probably be singular
private mapping VALID_MONEY =  ([ "gold" : 1, "zorkmid" : 3 ]);


//:FUNCTION query_exchange_rate
//Returns the exchange rate to map from an object's inherent value to its
//actual value in a particular currency.
int query_exchange_rate(string type)
{
    return VALID_MONEY[type];
}

//:FUNCTION query_valid_currencies
//Returns the names of the currencies that are valid within the mud
string *query_valid_currencies()
{
    return keys(VALID_MONEY);
}
