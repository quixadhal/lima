/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_PARSING;

mixed direct_push_obj(object ob) {
    return 1;
}

void do_push_obj(object ob, string name) {
    if (!ob->press(name)) {
	write( useless( "Pushing "+ob->the_short() ) );
    }
}

void do_push_obj_with_obj(object ob1, object ob2) {
    ob2->use("press", ob1);
}

void do_push_obj_str(object ob, string str) {
    ob->push_str(str);
}

mixed *query_verb_info() {
    return ({ ({ "OBJ", "OBJ STR", "OBJ with OBJ" }) });
}

