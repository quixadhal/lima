/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>

inherit VERB_OB;

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

//### see comments in get.
mixed can_drop_wrd_str(string amount, string str) {
    int z;
    string s1, s2;
    
    sscanf(amount, "%d%s", z, s1);
    if (s1 != "") return 0;
    sscanf(str, "%s %s", s1, s2);
    if (s2) {
	if (s2 != "coin" && s2 != "coins")
	    return 0;
	return 1;
    }
    return str == "coin" || str == "coins";
}

void do_drop_wrd_str(string amount, string str)
{
    string s;
    object ob;
    int number;

    /* Can't fail */
    sscanf(amount, "%d", number);
    
    sscanf(str, "%s %s", str, s);
    
    if (number < 0) {
	write("Nice try.\n");
	return;
    }
    if(this_body()->query_amt_money(str) < number) {
	write("You dont have "+ number +" "+ str +" coins.\n");
	return;
    } else {
	this_body()->subtract_money(str,number);
	if(ob = present("coins",environment(this_body()))) {
	    ob->merge_coins(number,str);
	} else {
	    new(COINS, number, str)->move(environment(this_body()));
	}
	this_body()->simple_action("$N $vdrop "+ number +" "+ str +" coins.");
    }
}

array query_verb_info()
{
    return ({ ({ "OBS", "WRD STR" }), ({ "put down" }) });
}
