/* Do not remove the headers from this file! see /USAGE for more info. */

inherit M_MESSAGES;

// Name is backwards compat
void do_player_message(string message, mixed arg) {
    mixed mess;
    
    mess = query_msg(message) || "$N $vdo not have a message for '"+message+"'.";

    if (pointerp(mess))
	mess = choice(mess);

    simple_action(mess, arg);
}

string *get_player_message(string message, mixed arg) {
    mixed mess;

    mess = query_msg(message) || "$N $vdo not have a message for '"+message+"'.";
    if (pointerp(mess))
	mess = choice(mess);
    return action( ({ this_object() }), mess, arg);
}
