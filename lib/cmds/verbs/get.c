/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** get.c
*/

#include <mudlib.h>
#include <move.h>
#include <setbit.h>

inherit NVERB_OB;

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
	this_body()->other_action("$N $vtake a $o.", ob);
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

void do_get_wrd_obj(string prep,object ob)
{
  ob->do_verb_rule("get", "WRD OBJ",prep,ob);
}

mixed can_get_wrd_str(string amount, string str) {
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
    return str == "coin" || str == "coins" || MONEY_D->is_denomination(str);
}

mixed can_get_wrd_str_from_obj(string amount, string str, object obj) {
    return can_get_wrd_str(amount, str);
}        

void do_get_wrd_str_from_obj(string amount, string str, object obj) {
    string s;
    // If there are two words, we want the first
    sscanf(str, "%s %s", str, s); 
    // direct_get_wrd_str_from_obj() or do_get_wrd_str() tested if
    // there is money there already
    present("money", obj)->get(amount, str); 
}

void do_get_wrd_str(string amount, string str) {
    object obj;

    if (obj = present("money", environment(this_body())))
        do_get_wrd_str_from_obj(amount, str, environment(this_body()));
    else
	write("There are no coins here.\n");
}

void create()
{
    add_rules( ({ "OBS", "WRD STR", "OBS from OBJ", "OBS out of OBJ",
"OBJ with OBJ", "WRD STR from OBJ", "WRD OBJ" }),
	       ({ "take", "carry", "pick up" }) );
}
