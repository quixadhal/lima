/* Do not remove the headers from this file! see /USAGE for more info. */

// Rewritten by Beek
// And converted again by Rust
#include <mudlib.h>

inherit CMD;
inherit M_MESSAGES;


private void main(mixed *arg, mapping flags)
{
    int i,j;
    object ob;
    string *msg;
    string *msgs;
    string *ob_msgs;
    int combat_msgs;
    int debug;
    string tmp;
    int flag;

    if (flags["c"]) combat_msgs = 1;
    if (flags["d"]) debug = 1;
    ob = arg[0];
    if (!ob) ob = this_body();
    msg = ob->query_msg_types();
    for (i=0; i<sizeof(msg); i++) {
        if (debug)
            write("["+msg[i]+"]");
	ob_msgs = ob->query_msg(msg[i]);
	if (!pointerp(ob_msgs)) ob_msgs = ({ ob_msgs });
	for (j=0; j<sizeof (ob_msgs); j++) {
            if (debug) write("["+ob_msgs[j]+"]");
            switch(msg[i]) {
	    case "miss":
            case "mleave":
	    case "light":
	    case "serious":
	    case "fatal":
	    case "disarm":
	    case "knockdown":
		if (!combat_msgs) continue;
msgs = action( ({ ob, this_object() }), ob_msgs[j], "something", "something");
		break;
            case "leave":
                msgs = action( ({ ob }), ob_msgs[j], "somewhere");
                break;
	    case "clone":
	    case "destruct":
		msgs = action( ({ ob }), ob_msgs[j], "something");
		break;
	    default:
		msgs = action( ({ ob }), ob_msgs[j]);
	    }
    write(msg[i] + ": [" + ob_msgs[j][0..<2] + "] ");
	    if (ob == this_body()) write(msgs[0]);
	    else write(msgs[<1]);
	    flag = 1;
	}
    }
    if (!flag) write("No messages.\n");
}

// so we can use this object as a generic object

string query_objective() { return "him"; }

string query_subjective() { return "he"; }

string query_named_possessive() { return "someone's"; }

string query_possessive() { return "his"; }

string short() { return "someone"; }
