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
    handle_obs(info, (: get_one :));
}

void do_get_obj_from_obj(object ob1, object ob2) {
    get_one(ob1, 0);
}

void do_get_obs_from_obj(array info, object ob2) {
    handle_obs(info, (: get_one :));
}

void do_get_obj_out_of_obj(object ob1, object ob2) {
    get_one(ob1, 0);
}

void do_get_obs_out_of_obj(array info, object ob2) {
    handle_obs(info, (: get_one :));
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
		  ".");
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

//### FIXME: These next two handle get 10 gold coins well enough if the
//### coins are in the room, but even get 10 gold coins from corpse will
//### fail.  What we probably want to do is have rules 'WRD OBJ from OBJ'
//### and 'WRD OBJ' and do something like ob->get_multiple(...).
mixed can_get_wrd_str(string amount, string str) {
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

void do_get_wrd_str(string amount, string str)
{
    string s;
    object ob;
    int number;

    /* Can't fail */
    sscanf(amount, "%d", number);

    sscanf(str, "%s %s", str, s); // If there are two words, we want the first

    if(ob = present("coins", environment(this_body()))) {
	ob->get(number, str);
	return;
    } else {
	this_body()->my_action("There are no coins here.\n");
    }
}

int do_get_off()
{
    "/cmds/verbs/stand"->do_stand();
}

array query_verb_info()
{
    return 
	({ 
	    ({ "WRD STR", "OBS from OBJ", "OBS out of OBJ",
		   "OBJ with OBJ" }), ({ "take", "carry" }),
	    ({ "off", "off OBJ", "on OBJ" }), ({ }),
	    ({ "OBS" }), ({ "pick up" })
	});
}
