/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** get.c
**
*/

#include <mudlib.h>
#include <move.h>
#include <setbit.h>

inherit VERB_OB;

// no multiple object support yet 
private nomask void get_one(object ob, object with_ob)
{
    mixed msg = ( with_ob ? ob->get_with(with_ob) : ob->get() );
    mixed tmp;

    if (!msg) msg = "You aren't able to take it.\n";
    if (stringp(msg)) {
	write(msg);
	return;
    }

    if(msg == MOVE_NO_ERROR)
    {
	return;
    }

    tmp = ob->move(this_body());
    if (tmp == MOVE_OK) {
	write("Taken.\n");
	ob->set_flag(TOUCHED);
	this_body()->other_action("$N $vtake a $o.\n", ob);
	return;
    }
    if (tmp == MOVE_NO_ERROR)
    {
	return;
    }

    if (tmp == MOVE_NO_ROOM) tmp = "Your load is too heavy.\n";
    if (!tmp) tmp = "That doesn't seem possible.\n";
    write(tmp);
}

void do_get_obj(object ob) {
    get_one(ob, 0);
}

void do_get_obs(object array info)
{
    foreach(mixed ob in info)
    {
	if (stringp(ob))
	    write(ob);
	else
	{
	    write(ob->short() + ": ");
	    get_one(ob, 0);
	}
    }
}

void do_get_obj_from_obj(object ob1, object ob2) {
    get_one(ob1, 0);
}

void do_get_obj_out_of_obj(object ob1, object ob2) {
    get_one(ob1, 0);
}

void do_get_obj_with_obj(object ob1, object ob2) {
    get_one(ob1, ob2);
}


void do_get_on_obj(object what)
{
    what->get_on();
}

void do_get_off_obj(object what)
{
    mixed s;
    string err;

    if(what != environment(this_body()))
    {
	write("You're not on it.\n");
	return;
    }
    if(s = what->get_off())
    {
	if(!stringp(err=this_body()->move(environment(what))))
	{
	    if(stringp(s))
		this_body()->simple_action(s);
	    else
		this_body()->simple_action("$N $vget off " + what->the_short()+
		  ".\n");
	}
	else
	{
	    write(err+"\n");
	}
    }
    else
    {
	write("You are already standing.\n");
    }
}

mixed do_get_wrd_str(string amount, string str)
{
    string *sentence = explode(str, " ");
    object ob;

    int number;

    if (sscanf(amount, "%d", number) == 1)
    {
	if(ob = present("coins", environment(this_body())))
	{
	    ob->get(number,sentence[0]);
	    return 1;
	}
	else
	{
	    this_body()->my_action("Get what what?\n");
	}
    }
    else
    {
	this_body()->my_action("Get what?\n");
    }
}

void do_get_up()
{
    mixed s;
    string err;

    if(s = environment(this_body())->stand())
    {
	if(!stringp(err=this_body()->move(environment(environment(this_body())))))
	{
	    if(stringp(s))
		this_body()->simple_action(s);
	    else
		this_body()->simple_action("$N $vstand up.\n");
	}
	else
	{
	    write(err+"\n");
	}
    }
    else
    {
	if(environment(environment(this_body())))
	{
	    write("You are unable.\n");
	}
	else
	{
	    write("You are already standing.\n");
	}
    }
}

int do_get_off()
{
    do_get_up();
}

mixed * query_verb_info()
{
    return 
	({ 
	    ({ "WRD STR", "OBJ from OBJ", "OBJ out of OBJ",
		   "OBJ with OBJ" }), ({ "take", "carry" }),
	    ({ "up", "off", "off OBJ", "on OBJ" }), ({ }),
	    ({ "OBJ", "OBS" }), ({ "pick up" })
	});
}
