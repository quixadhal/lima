/* Do not remove the headers from this file! see /USAGE for more info. */

object query_weapon();
object query_target();
void simple_action(string);
varargs mixed *action(mixed *, mixed, object, object);
void inform(mixed *, mixed, object);

//:FUNCTION print_result
//Print the result of a round of combat
void print_result(string result) {
    string *msgs;
    string message;
    object *combat_who;
    
    if (!result) return;
    message = query_weapon()->query_combat_message(result);

    if (!message)
    {
	simple_action("$N $vare puzzled because $n $vhave no message for '"+result+"'.\n");
    }
    else
    {
	combat_who = ({ this_object(), query_target() });
        if (result == "disarm")
	    msgs = action( combat_who, message, query_weapon(), combat_who[1]->query_weapon());
        else
            msgs = action( combat_who, message, query_weapon());
	inform( combat_who, msgs, environment() );
    }
}

string negotiate_result(string result, string type) {
    result = query_weapon()->adjust_my_result(result, type);
    result = query_target()->adjust_result(result, type);
    return result;
}
