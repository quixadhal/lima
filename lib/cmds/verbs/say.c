/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove headers from this file! see /USAGE for more info. */

/* 
 * Converted from command to verb 05052000 -- Tigran
 */

inherit VERB_OB;
inherit M_GRAMMAR;

mixed can_say_str() {
    return 1;
}

mixed do_say_str(string str) {
    string array msgs;
    object array others = ({});
    object ob;

    // Collect speech recipients upwards through environments,
    // where proper. Not possible to use normal message propagation
    // if you want to save say history in the body object.
    // -- Marroc
    ob = this_body();
    while (ob && ob->environment_can_hear()) {
	others += all_inventory(environment(ob)) - ({ ob });
	ob = environment(ob);
    }

    switch(explode(str," ")[0]) {
	string array out=({});
    case "/last":
    case "/history":
	out=({"History of says:\n"});
	msgs=this_body()->list_say_history();
	if(sizeof(msgs))
	    out+=msgs;
	else
	    out+=({"\t<none>"});
	more(out);
	break;
    default:
	msgs=this_body()->action(({this_body()}),
	  "%^SAY%^$N $vsay:%^RESET%^ $o",
	  punctuate(str));
	this_body()->inform(({this_body()}),msgs,others);
	this_body()->add_say_history(msgs[0]);
	others->add_say_history(msgs[1]);
    }
}

void create(mixed args...) {
    add_rules( ({"STR"}), ({}) );
}

/* 
 * Converse compatability stuff
 */
nomask int valid_resend(string ob) {
    return ob == "/cmds/player/converse";
}

nomask void do_resend(mixed arg1, mixed arg2) {
    do_say_str(arg2);
}
