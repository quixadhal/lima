/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** drop.c
**
*/

#include <mudlib.h>
#include <move.h>

inherit VERB_OB;

mixed can_drop_obj(object ob)
{
    return 1;
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
	this_body()->other_action("$N $vdrop a $o.\n", ob);
    } else
	write(tmp);
}

void do_drop_obj(object ob)
{
    drop_one(ob);
}




mixed do_drop_wrd_str(string amount, string str)
{
    object ob;
    string *sentence = explode(str," ");
    int number;

    if( sscanf(amount, "%d", number) == 1)
    {
	if(this_body()->query_amt_money(sentence[0]) < number)
	{
	    write("You dont have "+ amount +" "+ sentence[0] +".\n");
	    return 0;
	}
	else
	{
	    this_body()->subtract_money(sentence[0],number);
	    if(ob = present("coins",environment(this_body())))
	    {
		ob->merge_coins(number,sentence[0]);
	    }
	    else
	    {
		new(COINS,number,sentence[0])->move(environment(this_body()));
	    }
	    this_body()->simple_action("$N $vdrop "+ number +" "+ sentence[0] +" coins.\n");
	}
    }
    else
	write("You can't drop that.\n");
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ", "OBS", "WRD STR" }) });

    /*
    ** torch OBJ when OBS matches 1 ob
    **
    ** "put down OBS" -> "drop OBS"
    */
}
