/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** give.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

void do_give_obj_to_liv(object ob, object liv) {
    if (!try_to_acquire(ob))
	return;
    if( ob->is_worn())
	write( "You'll need to remove it first.\n");
    else
    {
	this_body()->targetted_action("$N $vgive a $o to $t.", liv, ob);
	//### Need to check the return value
	ob->move(liv);
    }
}

void do_give_obs_to_liv(array info, object liv) {
    handle_obs(info, (: do_give_obj_to_liv :), liv);
}

//##see comments in get
mixed can_give_wrd_str_to_liv(string amount, string str, object who) {
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

void do_give_wrd_str_to_liv(string amount, string str, object who)
{
    string s;
    int number;

    sscanf(amount, "%d", number);

    sscanf(str, "%s %s", str, s);

    if(this_body()->query_amt_money(str) < number) {
	write("You don't have "+ number +" "+ str +" coins.\n");
	return;
    } else {
	who->add_money(str,number);
	this_body()->subtract_money(str,number);
	this_body()->simple_action("$N $vgive $o some money.",who);
    }
}

array query_verb_info()
{
    return ({ ({ "OBS to LIV", "WRD STR to LIV" }),
      ({ "hand" }) });
}
