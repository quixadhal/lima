/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>

inherit NVERB_OB;

mixed can_drop_obj() {
    // give a better message for this case, since all the errors generated
    // will be nonsense.
    if (!first_inventory(this_body()))
	return "You don't have anything.\n";
    return default_checks();
}

private nomask void drop_one(object ob)
{
    mixed tmp = ob->drop();

    if (!tmp) tmp = "You aren't able to drop it.\n";
    if (stringp(tmp)) {
	write(tmp);
	return tmp;
    }

    tmp = ob->move(environment(this_body()));
    if (tmp == MOVE_OK) {
	write("Done.\n");
        if(ob)
  	this_body()->other_action("$N $vdrop a $o.", ob);
    } else
	write(tmp);
}

void do_drop_obj(object ob)
{
    drop_one(ob);
}

void do_drop_obs(array info)
{
    foreach (mixed item in info)
    {
	if (stringp(item))
	    write(item);
	else
	{
	    write(item->short() + ": ");
	    drop_one(item);
	}
    }
}

mixed can_drop_wrd_str(string amount, string str) {
    int z;
    string s1, s2;
    
    sscanf(amount, "%d%s", z, s1);
    if (s1 != "" && amount != "all") 
        return 0;
    sscanf(str, "%s %s", s1, s2);
    if (s2) {
	if (s2 != "coin" && s2 != "coins")
	    return 0;
	return MONEY_D->is_denomination(s1);
    }
    return MONEY_D->is_denomination(str);
}

void do_drop_wrd_str(string amount_str, string type)
{
    string s;
    object ob;
    int amount;

    sscanf(type, "%s %s", type, s);
    type = MONEY_D->singular_name(type);
    if (amount_str == "all")
        amount = this_body()->query_amt_money(type);
    else
        amount = to_int(amount_str);
    if (amount < 0) {
	write("Nice try.\n");
	return;
    }
    if(this_body()->query_amt_money(type) < amount) {
	write("You don't have "
	      +MONEY_D->denomination_to_string(amount, type)+".\n");
	return;
    } else {
	this_body()->subtract_money(type, amount);
	if(ob = present("money", environment(this_body())))
#ifdef USE_MONEY
	    ob->merge_money(amount, type);
	else
	    new(MONEY, amount, type)->move(environment(this_body()));
#else
	    ob->merge_coins(amount, type);
	else
	    new(COINS, amount, type)->move(environment(this_body()));
#endif
	this_body()->simple_action("$N $vdrop "
		       +MONEY_D->denomination_to_string(amount, type)+".");
    }
}

void create()
{
    add_rules( ({ "OBS", "WRD STR" }), ({ "put down" }) );
}
