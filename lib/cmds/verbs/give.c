/* Do not remove the headers from this file! see /USAGE for more info. */


inherit VERB_OB;

void do_give_obj_to_liv(object ob, object liv) {
  mixed msg=ob->move(liv);
  if(stringp(msg)) {
    write(msg);
    return;
  }
  if(msg==1)
    this_body()->targetted_action("$N $vgive a $o to $t.", liv, ob);
}

void do_give_obs_to_liv(array info, object liv) {
    handle_obs(info, (: do_give_obj_to_liv :), liv);
}

mixed can_give_wrd_str_to_liv(string amount, string str, object who) {
    int z;
    string s1, s2;

    sscanf(amount, "%d%s", z, s1);
    if (s1 != "" && amount != "all") return 0;

    sscanf(str, "%s %s", s1, s2);
    if (s2) {
	if (s2 != "coin" && s2 != "coins")
	    return 0;
        return MONEY_D->is_denomination(s1);
    }
    return MONEY_D->is_denomination(str);
}

void do_give_wrd_str_to_liv(string amount, string str, object who)
{
    string s;
    int number;

    sscanf(str, "%s %s", str, s);
    str = MONEY_D->singular_name(str);
    if (amount == "all")
        number = this_body()->query_amt_money(str);
    else
        number = to_int(amount);
    if (number < 0) {
        write("Nice try.\n");
	return;
    }
    if(this_body()->query_amt_money(str) < number) {
	write("You don't have "
	      +MONEY_D->denomination_to_string(number, str)+".\n");
	return;
    } else {
	who->add_money(str,number);
	this_body()->subtract_money(str,number);
	this_body()->other_action("$N $vgive $o some money.", who);
	this_body()->my_action("$N $vgive $o "
			       +MONEY_D->denomination_to_string(number, str)
			       +".", who);
    }
}

void create()
{
    add_rules( ({ "OBS to LIV", "WRD STR to LIV" }), ({ "hand" }) );
}
