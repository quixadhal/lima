/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

/* ### this probably shouldn't be here... but for now (simul conversion)... */
private inherit M_GRAMMAR;

/* General message handling.  Inherit it in anything that needs it.
 *
 * -Beek
 */

#define A_SHORT(x) (objectp(x) ? x->a_short() : x)
#define SHORT(x) (objectp(x) ? x->short() : x)
#define THE_SHORT(x) (objectp(x) ? x->the_short() : x)
mapping messages;

void add_msg(string cls, string msg) {
    if (!messages) messages = ([]);
    if (pointerp(messages[cls]))
	messages[cls] += ({ msg });
    else if (messages[cls]) {
	messages[cls]=({ messages[cls], msg });
    } else
	messages[cls]=msg;
}

string query_msg(string which) { return messages[which]; }

void set_msgs(string cls, string *msgs) {
    if (!messages) messages = ([]);
    messages[cls]= msgs;
}

string *query_msg_types() {
    return keys(messages);
}


string compose_message(object forwhom, string msg, object *who, mixed ob0, mixed ob1) {
    mixed ob;
    mixed *fmt;
    string res;
    int i;
    int c;
    int num;
    string str;
    string bit;
    int has_o, has_n, has_t;

    fmt = reg_assoc(msg, ({ "\\$[NnVvTtPpOoRr][a-z0-9]*" }), ({ 1 }) );
    fmt = fmt[0]; // ignore the token info for now

	res = fmt[0];
    i=1;
    while (i<sizeof(fmt)) {
	c = fmt[i][1];
	if (fmt[i][2] && fmt[i][2]<'a') {
	    num = fmt[i][2] - '0';
	    str = fmt[i][3..<0];
	} else {
	    num =0;
	    str = fmt[i][2..<0];
	}
	switch (c) {
	case 'o':
	case 'O':
	    if (!num && objectp(ob0) && has_o) bit = "it";
	    else {
		ob = num ? ob1 : ob0;
		if (res[<2..<1]=="a ") {
		    res = res[0..<3];
		    bit = A_SHORT(ob);
		} else if (res[<4..<1] == "the ") {
		    res = res[0..<5];
		    bit = THE_SHORT(ob);
		} else bit = SHORT(ob);
		if (!num) has_o = 1;
	    }
	    break;
	case 't':
	case 'T':
	    num = 1;
	    if (str=="") str = "o";
	    if (str != "p" && has_n && (who[0] == who[1])) {
		// objective: You kick yourself, Beek kicks himself.
		if (str == "o") {
		    if (forwhom == who[0]) bit = "yourself";
		    else bit = who[0]->query_reflexive();
		}
		// subjective: You prove you are stupid,
		// Beek proves he is stupid.
		if (str == "s") {
		    if (forwhom == who[0]) bit = "you";
		    else bit = who[0]->query_subjective();
		}
		break;
	    }
	    /* Fall through */
	case 'n':
	case 'N':
	    if (str != "p") {
		if (who[num]==forwhom) {
		    bit = "you";
		    if (!num) has_n = 1;
		    else if (num==1) has_t = 1;
		    break;
		}
		if ((num==0 && has_n) || (num==1 && has_t)) {
		    if (str[0]=='o') bit = who[num]->query_objective();
		    else bit = who[num]->query_subjective();
		    break;
		}
	    }
	    if (!num) has_n=1;
	    else if (num==1) has_t=1;
	    bit = who[num]->short();
	    break;
	case 'R':
	case 'r':
	    if (forwhom == who[num])
		bit = "yourself";
	    else
		bit = who[num]->query_reflexive();
	    break;
	case 'v':
	case 'V':
	    if (who[num]!=forwhom) bit = pluralize(str);
	    else bit = str;
	    break;
	case 'p':
	case 'P':
	    if (forwhom == who[num]) {
		bit = "your";
		break;
	    }
	    if ((num==0 && has_n) || (num==1 && has_t)) {
		bit = who[num]->query_possessive();
		break;
	    }
	    bit = who[num]->query_named_possessive();
	    if (num==0) has_n=1;
	    else if (num==1) has_t=1;
	    break;
	}
	// hack to prevent errors.
	if (!bit) bit = "";
	if (c < 'a') bit = capitalize(bit);
	res += bit + fmt[i+1];
	i+=2;
    }
    if (res[<1] == '.') res = punctuate(res[0..<2]);
    return res;
}

varargs string *action(object *who, mixed msg, object ob, object ob2) {
    int i;
    string *res;

    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    res = allocate(sizeof(who)+1);
    for (i=0; i<sizeof(who); i++) {
	res[i] = compose_message(who[i], msg, who, ob, ob2);
    }
    res[sizeof(who)]=compose_message(0, msg, who, ob, ob2);
    return res;
}

void inform(object *who, string *msgs, mixed others) {
    int i;
    for (i=0; i<sizeof(who); i++) {
	if (i && who[i]==who[0]) continue;
	tell_object(who[i], iwrap(msgs[i]));
    }
    if (pointerp(others))
    {
	map_array(others, (: tell_object($1, $(iwrap(msgs[<1]))) :));
    }
    else if (others)
    {
	tell_room(others, iwrap(msgs[sizeof(who)]), who);
    }
}

varargs void simple_action(mixed msg, object ob, object ob2) {
    string us;
    string others;
    object *who;

    /* faster to only make who once */
    who = ({ this_object() });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    us = compose_message(this_object(), msg, who, ob, ob2);
    others = compose_message(0, msg, who, ob, ob2);

    tell_object(this_object(), iwrap(us));
    if (environment())
	tell_room(environment(), iwrap(others), who);
}

varargs void my_action(mixed msg, object ob, object ob2) {
    string us;
    object *who;

    who = ({ this_object() });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    us = compose_message(this_object(), msg, who, ob, ob2);
    tell_object(this_object(), iwrap(us));
}

varargs void other_action(mixed msg, object ob, object ob2) {
    string others;
    object *who;

    who = ({ this_object() });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    others = compose_message(0, msg, who, ob, ob2);
    tell_room(environment(), iwrap(others), who);
}

varargs void targetted_action(mixed msg, object target, object ob, object ob2) {
    string us, them, others;
    object *who;

    who = ({ this_object(), target });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    us = compose_message(this_object(), msg, who, ob, ob2);
    them = compose_message(target, msg, who, ob, ob2);
    others = compose_message(0, msg, who, ob, ob2);
    tell_object(this_object(), iwrap(us));
    tell_object(target, iwrap(them));
    tell_room(environment(), iwrap(others), who);
}
