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

    if(ob->gettable() && !try_to_acquire(ob))
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
    return ({ ({ "OBJ:v", "from OBJ:v", "in OBJ:v",
		     "about STR in OBJ:v", "about STR from OBJ:v",
		     "STR in OBJ:v", "STR from OBJ:v"
	     }) });
}
