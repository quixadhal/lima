/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//
// Syntax: <say MESSAGE>
//         <say /last>
//
// This command is used to talk with others in the same room as you.
// It is done in a common language that everybody can understand.
// The /last and /history syntaxes will display your say history.

inherit CMD;
inherit M_GRAMMAR;

#ifdef SAY_HISTORY_IN_ROOMS
# define HISTORY_OB environment(this_body())
#else
# define HISTORY_OB this_body()
#endif

void create()
{
    ::create();
    no_redirection();
}

private void main(string str)
{
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

    if (!str || str == "")  {
	out("Say what?\n");
	return;
    }
    switch(explode(str," ")[0]) {
	string array out=({});
    case "/last":
    case "/history":
	out=({"History of says:\n"});
	msgs=HISTORY_OB->list_say_history();
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
#ifndef SAY_HISTORY_IN_ROOMS
	HISTORY_OB->add_say_history(msgs[0]);
	others->add_say_history(msgs[1]);
#else
	HISTORY_OB->add_say_history(msgs[1]);
#endif
    }
}

nomask int valid_resend(string ob) {
    return ob == "/cmds/player/converse";
}
