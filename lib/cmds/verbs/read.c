/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;


private object* get_all_readable_entry_items()
{
    object * res = ({});
    object * search;
    search = all_inventory(environment(this_body())) + all_inventory(this_body());
    while(sizeof(search))
    {
	res += filter(search, (:$1->has_entries():));
	search = filter(search, (:$1->inventory_accessable() && !$1->is_living():));
	search = decompose(map(search, (:all_inventory:)));
    }
    return res;
}

mixed can_read_obj(object ob) {
    return check_vision();
}

mixed can_read_word_obj(string p, object ob) {
    if (p != "in" && p != "from") return 0;
    return check_vision();
}

mixed can_read_word_str(string p, string s) {
    if (p != "about") return 0;
    return check_vision();
}

mixed can_read_str_word_obj(string str, string p, object ob) {
    if (p != "in" && p != "from") return 0;
    return check_vision();
}

mixed can_read_word_str_word_obj(string p1, string str, string p2, object ob) {
    if (p1 != "about") return 0;
    if (p2 != "in" && p2 != "from") return 0;
    return check_vision();
}


static void read_it(object ob, string str) {
    string text;

    if(ob->is_gettable() && !try_to_acquire(ob))
	return;

    if (str)
	text = ob->read_entry(str);
    else
	text = ob->read();

    more(text);
}

void do_read_obj(object ob) {
    read_it(ob, 0);
}

void do_read_word_obj(string prep, object ob) {
    read_it(ob, 0);
}

void do_read_str_word_obj(string str, string p, object ob) {
    read_it(ob, str);
}

void do_read_word_str_word_obj(string p1, string str, string p2, object ob) {
    read_it(ob, str);
}

void do_read_word_str(string p, string str) {
    object* assumptions;

    assumptions = get_all_readable_entry_items();
    switch(sizeof(assumptions))
    {
    case 0:
	write("There is nothing here in which to look that up.\n");
	return;
    case 1:
	printf("[from %s]\n", assumptions[0]->the_short());
	read_it(assumptions[0], str);
	return;
    default:
	write("I'm not clear on what you're trying to read from.\n");
	return;
    }
}



array query_verb_info()
{
    return ({ ({ "OBJ:v", "from OBJ:v", "in OBJ:v", "about STR",
	"about STR in OBJ:v", "about STR from OBJ:v",
	"STR in OBJ:v", "STR from OBJ:v", 
      }) });
}
