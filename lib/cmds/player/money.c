/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;
private void main()
{
    int i;
    int amount;
    string currency;
    string retmsg;
    string *types;

    types = this_body()->query_currencies();
    i = sizeof(types);
    if(!i)
	write("You dig around in your pockets and find only lint!\n");
    else
    {
	write ("You have the following types of money:\n");
	while(i--)
	{
	    currency = types[i];
	    amount = this_body()->query_amt_money(currency);
	    printf("\n%10s %-30i\n", currency,amount);
	}
    }
}
