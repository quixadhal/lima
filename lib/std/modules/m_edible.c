/* Do not remove the headers from this file! see /USAGE for more info. */

private int num_eats = 1;
private int original_eats;
private mixed eat_action = "$N $veat $o.";
private mixed last_eat_action;

string the_short();

//:FUNCTION set_eat_action
//set_eat_action(string) causes 'string' to be printed when someone
//eats this object.  set_eat_action(function) causes the function
//to be called instead.
void set_eat_action(mixed action) {
    eat_action = action;
}

//:FUNCTION set_last_eat_action
//Just like set_eat_action, but only is used for the last eat
void set_last_eat_action(mixed action) {
    last_eat_action = action;
}

//:FUNCTION set_num_eats
//num_eats is the number of eats left before the object is empty.
void set_num_eats(int num) {
original_eats = num;
    num_eats = num;
}

int get_num_eats()
{
    return num_eats;
}

int get_original_eats()
{
    return original_eats;
}

mixed direct_eat_obj() {
    if (!num_eats)
	return capitalize(the_short()) + " is gone.\n";

    return 1;
}

void do_eat()
{
    mixed action;

    if( num_eats == 1 )
    {
	if( last_eat_action ) action = last_eat_action;
	else action = eat_action;
	if( stringp( action ))
	    this_body()->simple_action( action, this_object());
	else evaluate( action );
	this_object()->remove();
    }
    else
    {

	if (stringp(eat_action))
	    this_body()->simple_action(eat_action, this_object());
	else
	    evaluate(eat_action);

	num_eats--;
    }
}
