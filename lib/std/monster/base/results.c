/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat.h>

inherit CLASS_COMBAT_RESULT;

object query_weapon();
object query_target();
void simple_action(string msg, array obs...);
varargs mixed *action(mixed *, mixed, object, object);
void inform(mixed *, mixed, object);
string default_message(int, int);

void unwield();
void die();
void stun();
void knock_out();
void hurt_us(int);

static void print_one_result(class combat_result result) {
    string *msgs;
    string message, mess;
    object *combat_who;

    if (!result || result->special & RES_NONE) return;
    if (!(mess = result->message)) {
	if (result->special) {
	    if (result->special & RES_FATAL)
		mess = "!fatal";
	    else if (result->special & RES_KNOCKDOWN)
		mess = "!knockdown";
	    else if (result->special & RES_KNOCKOUT)
		mess = "!knockdown";
	    else 
		mess = default_message(result->damage, query_target()->query_hp());
	} else
	    mess = default_message(result->damage, query_target()->query_hp());
    }
    if (mess[0] == '!')
	message = query_weapon()->query_combat_message(mess[1..]);
    else
	message = mess;

    if (!message)
    {
	simple_action("$N $vare puzzled because $n $vhave no message for '"+mess[1..]+"'.");
    }
    else
    {
	//### FIXME: This shouldn't be here; it defeats the purpose of caching it.
	combat_who = ({ this_object(), query_target() });

	msgs = action( combat_who, message, query_weapon(), combat_who[1]->query_weapon());
	inform( combat_who, msgs, environment() );
    }
}

//:FUNCTION print_result
//Print the result of a round of combat
void print_result(class combat_result array arg) {
    foreach (mixed tmp in arg)
    print_one_result(tmp);
}

class combat_result array negotiate_result(class combat_result array result) {
    result = query_weapon()->adjust_my_result(result);
    result = query_target()->adjust_result(result);
    return result;
}

static void do_one_result(class combat_result res) {
    int s = res->special;

    if (s & RES_NONE)
	return;
    if (s & RES_DISARM) {
	object w = query_weapon();
	unwield();
	w->move(environment());
    }
    if (s & RES_FATAL) {
	die();
	return;
    }
    if (s & RES_KNOCKDOWN)
	stun();
    if (s & RES_KNOCKOUT)
	knock_out();

    hurt_us(res->damage);
}

/* This is actually a misnomer.  It does the result of a round.
 * This is the function to call if you want to do damage to a
 * player (from a spell, etc).
 */
//:FUNCTION do_damage
//Apply a combat result to us.
void do_damage(mixed arg) {
    if (arrayp(arg))
	foreach (mixed res in arg)
	    do_one_result(res);
    else
	do_one_result(arg);
}

