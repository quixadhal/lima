/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_PARSING;

mixed direct_pull_obj(object ob) {
    return 1;
}

void do_pull_obj(object ob, string name) {
    if (!ob->pull(name)) {
        write( useless( "Pulling "+ob->the_short() ) );
    }
}

void do_pull_obj_with_obj(object ob1, object ob2) {
    ob2->use("pull", ob1);
}

void do_pull_obj_str(object ob, string str) {
    ob->pull_str(str);
}

array query_verb_info() {
    return ({ ({ "OBJ", "OBJ STR", "OBJ with OBJ" }), ({ "yank" }) });
}
