/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

/* General message handling.  Inherit it in anything that needs it.
 *
 * -Beek
 */

//:MODULE
// The message module.  The correct way to compose and send any messages
// To users is using this module, as it will automatically get the grammar
// right for each person involved.

/* More simul conversion fall out */
string punctuate(string);
private string vowels = "aeiouAEIOU";

#define A_SHORT(x) (objectp(x) ? x->a_short() : (member_array(x[0], vowels) == -1 ? "a " : "an ") + x)
#define SHORT(x) (objectp(x) ? x->short() : x)
#define THE_SHORT(x) (objectp(x) ? x->the_short() : "the " + x)
mapping messages = ([]);
mapping def_messages = ([]);

void set_def_msgs(string type) {
    if (!(def_messages = MESSAGES_D->get_messages(type)))
	error("No messages of that type.\n");
}

void add_msg(string cls, string msg) {
    if (!messages) messages = ([]);
    if (pointerp(messages[cls]))
	messages[cls] += ({ msg });
    else if (messages[cls]) {
	messages[cls]=({ messages[cls], msg });
    } else
	messages[cls]=msg;
}

string query_msg(string which) { return messages[which] || def_messages[which]; }

void set_msgs(string cls, string *msgs) {
    if (!messages) messages = ([]);
    if(!msgs || !sizeof(msgs))  {
         map_delete(messages, cls);
        return;
    }
    messages[cls] = msgs;
}

void clear_messages() {
    messages = ([]);
}

string *query_msg_types() {
    return keys(messages) + keys(def_messages);
}

//:FUNCTION compose_message
//The lowest level message composing function; it is passed the object
//for whom the message is wanted, the message string, the array of people
//involved, and the objects involved.  It returns the appropriate message.
//Usually this routine is used through the higher level interfaces.

varargs string compose_message(object forwhom, string msg, object *who, 
			       array obs...) {
    mixed ob;
    mixed *fmt;
    string res;
    int i;
    int c;
    int num, subj;
    string str;
    string bit;
    mapping has = ([]);

    fmt = reg_assoc(msg, ({ "\\$[NnVvTtPpOoRr][a-z0-9]*" }), ({ 1 }) );
    fmt = fmt[0]; // ignore the token info for now

    res = fmt[0];
    i=1;
    while (i<sizeof(fmt)) {
	c = fmt[i][1];
	if (fmt[i][2] && fmt[i][2]<'a') {
	    if (fmt[i][3] && fmt[i][3] < 'a') {
		subj = fmt[i][2] - '0';
		num = fmt[i][3] - '0';
		str = fmt[i][4..<0];
	    } else {
		subj = 0;
		num = fmt[i][2] - '0';
		str = fmt[i][3..<0];
	    }
	} else {
	    subj = 0;
	    num = ((c == 't' || c == 'T') ? 1 : 0); // target defaults to 1, not zero
	    str = fmt[i][2..<0];
	}
	switch (c) {
	case 'o':
	case 'O':
	    ob = obs[num];
	    if (objectp(ob) && has[ob]) bit = "it";
	    else {
		if (res[<2..<1]=="a ") {
		    res = res[0..<3];
		    bit = A_SHORT(ob);
		} else if (res[<4..<1] == "the ") {
		    res = res[0..<5];
		    bit = THE_SHORT(ob);
		} else if (res[<2..<1] == "A ") {
		    res = res[0..<3];
		    bit = capitalize(A_SHORT(ob));
		} else if (res[<4..<1] == "The ") {
		    res = res[0..<5];
		    bit = capitalize(THE_SHORT(ob));
		  } else bit = SHORT(ob);
		has[ob]++;
	    }
	    break;
	case 't':
	case 'T':
	    /* Only difference between $n and $t is that $t defaults to $n1o */
	    /* Fall through */
	    if (str=="") str = "o";
	case 'n':
	case 'N':
	    if (str=="") str = "s";
	    if (str != "p") {
		/* Handle reflexification */
		if (subj < sizeof(who) &&
		    (who[subj] == who[num]) && has[who[subj]]) {
		    // objective: You kick yourself, Beek kicks himself.
		    if (str == "o") {
			if (forwhom == who[subj]) bit = "yourself";
			else bit = who[subj]->query_reflexive();
		    }
		    // subjective: You prove you are stupid,
		    // Beek proves he is stupid.
		    if (str == "s") {
			if (forwhom == who[subj]) bit = "you";
			else bit = who[subj]->query_subjective();
		    }
		    break;
		}
		/* Other pronouns */
		if (who[num]==forwhom) {
		    bit = "you";
		    has[who[num]]++;
		    break;
		}
		if (has[who[num]]) {
		    if (str[0]=='o') bit = who[num]->query_objective();
		    else bit = who[num]->query_subjective();
		    break;
		}
	    }
	    has[who[num]]++;
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
	    if (num >= sizeof(who) || who[num]!=forwhom) bit = M_GRAMMAR->pluralize(str);
	    else bit = str;
	    break;
	case 'p':
	case 'P':
	    if (forwhom == who[num]) {
		bit = "your";
		break;
	    }
	    if (has[who[num]]) {
		bit = who[num]->query_possessive();
		break;
	    }
	    bit = who[num]->query_named_possessive();
	    has[who[num]]++;
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

//:FUNCTION action
//Make the messages for a given group of people involved.  The return
//value will have one array per person, as well as one for anyone else.
//inform() can be used to send these messages to the right people.
//see: inform

varargs string *action(object *who, mixed msg, array obs...) {
    int i;
    string *res;

    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    res = allocate(sizeof(who)+1);
    for (i=0; i<sizeof(who); i++) {
	res[i] = compose_message(who[i], msg, who, obs...);
    }
    res[sizeof(who)]=compose_message(0, msg, who, obs...);
    return res;
}

//:FUNCTION inform
//Given an array of participants, and an array of messages, and either an
//object or array of objects, deliver each message to the appropriate
//participant, being careful not to deliver a message twice.
//The last arg is either a room, in which that room is told the 'other'
//message, or an array of people to recieve the 'other' message.
void inform(object *who, string *msgs, mixed others) {
    int i;
    mapping done = ([]);
    for (i=0; i<sizeof(who); i++) {
        if (done[who[i]]) continue;
        done[who[i]]++;
	tell_object(who[i], iwrap(msgs[i]));
    }
    if (pointerp(others))
    {
	map_array(others - who, (: tell_object($1, $(iwrap(msgs[<1]))) :));
    }
    else if (others)
    {
	tell_room(others, iwrap(msgs[sizeof(who)]), 0, who);
    }
}

//:FUNCTION simple_action
//Generate and send messages for an action involving the user and possibly
//some objects
varargs void simple_action(mixed msg, array obs...) {
    string us;
    string others;
    object *who;

    /* faster to only make who once */
    who = ({ this_object() });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    us = compose_message(this_object(), msg, who, obs...);
    others = compose_message(0, msg, who, obs...);

    tell_object(this_object(), iwrap(us));
    if (environment())
	tell_room(environment(), iwrap(others), 0, who);
}

//:FUNCTION my_action
//Generate and send a message that should only be seen by the person doing it
varargs void my_action(mixed msg, array obs...) {
    string us;
    object *who;

    who = ({ this_object() });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    us = compose_message(this_object(), msg, who, obs...);
    tell_object(this_object(), iwrap(us));
}

//:FUNCTION other_action
//Generate and send a message that should only be seen by others
varargs void other_action(mixed msg, array obs...) {
    string others;
    object *who;

    who = ({ this_object() });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    others = compose_message(0, msg, who, obs...);
    tell_room(environment(), iwrap(others), 0, who);
}

//:FUNCTION targetted_action
//Generate and send a message involving the doer and a target (and possibly
//other objects)
varargs void targetted_action(mixed msg, object target, array obs...) {
    string us, them, others;
    object *who;

    who = ({ this_object(), target });
    if (pointerp(msg))
	msg = msg[random(sizeof(msg))];
    us = compose_message(this_object(), msg, who, obs...);
    them = compose_message(target, msg, who, obs...);
    others = compose_message(0, msg, who, obs...);
    tell_object(this_object(), iwrap(us));
    tell_object(target, iwrap(them));
    tell_room(environment(), iwrap(others), 0, who);
}
