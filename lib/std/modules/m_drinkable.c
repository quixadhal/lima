/* Do not remove the headers from this file! see /USAGE for more info. */

private int num_drinks;
private mixed drink_action;
private mixed last_drink_action;

string the_short();

//:FUNCTION set_drink_action
//set_drink_action(string) causes 'string' to be printed when someone
//drinks this object.  set_drink_action(function) causes the function
//to be called instead.  Note that if num_drinks is more than one, then
//last_drink_action is used instead for the last drink, if it is set.
void set_drink_action(mixed action) {
    drink_action = action;
}

//:FUNCTION set_last_drink_action
//Just like set_drink_action, but only is used for the last drink.
void set_last_drink_action(mixed action) {
    last_drink_action = action;
}

//:FUNCTION set_num_drinks
//num_drinks is the number of drinks left before the object is gone.
void set_num_drinks(int num) {
    num_drinks = num;
}

mixed direct_drink_obj() {
    if (!num_drinks)
	return capitalize(the_short()) + " is gone.\n";

    return 1;
}

void drink_it() {
    mixed action;
    
    if (num_drinks == 1 && last_drink_action)
	action = last_drink_action;
    else
	action = drink_action;

    if (stringp(action))
	write(action);
    else
	evaluate(action);
	
    num_drinks--;
}
