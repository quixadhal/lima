/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** read.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

mixed can_read_obj(object ob) {
    return check_vision();
}

mixed can_read_prep_obj(string p, object ob) {
    if (p != "in" && p != "from") return 0;
    return check_vision();
}

mixed can_read_str_prep_obj(string str, string p, object ob) {
    if (p != "in" && p != "from") return 0;
    return check_vision();
}

mixed can_read_prep_str_prep_obj(string p1, string str, string p2, object ob) {
    if (p1 != "about") return 0;
    if (p2 != "in" && p2 != "from") return 0;
    return check_vision();
}


static void read_it(object ob, string str) {
    string text;

    if (!try_to_acquire(ob))
	return;

    if (str)
	text = ob->read_entry(str);
    else
        text = ob->read();
    if (sizeof(text) > 200) {
	// Don't use more on short strings
	clone_object(MORE_OB)->more_string(text);
    } else {
	write(text);
    }
    if (str)
	this_body()->other_action("$N $vread about $o0 from a $o1.\n", ob, str);
    else
        this_body()->other_action("$N $vread a $o.\n", ob);
}

void do_read_obj(object ob) {
    read_it(ob, 0);
}

void do_read_prep_obj(string prep, object ob) {
    read_it(ob, 0);
}

void do_read_str_prep_obj(string str, string p, object ob) {
    read_it(ob, str);
}

void do_read_prep_str_prep_obj(string p1, string str, string p2, object ob) {
    read_it(ob, str);
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ", "from OBJ", "in OBJ",
		     "about STR in OBJ", "about STR from OBJ",
		     "STR in OBJ", "STR from OBJ"
	     }) });
}
