/* Do not remove the headers from this file! see /USAGE for more info. */
//:MODULE
//This module is almost obsolete.
//Use m_fluid_container instead, unless
//you want no fluid associated with your
//drink container.
private int needs_contents;

string the_short();
int query_drinks();

//:FUNCTION set_needs_contents
//If needs_contents is 1, then the container can only be drunk from if it
//contains a drink.  Otherwise the 'drink' is assumed to be handled as
//part of the container itself (either by also inheriting M_DRINKABLE or
//otherwise).
void set_needs_contents(int x) {
    needs_contents = x;
}

mixed direct_drink_from_obj() {
    object ob;
    
    if (this_object()->query_closed())
	return capitalize(the_short()) + " is closed.\n";

    if (needs_contents)
	ob = first_inventory();
    else
        ob = this_object();
    
    if (!ob)
	return capitalize(the_short()) + " is empty.\n";

    return ob->direct_drink_obj();
}

void drink_from_it() {
    object ob;
    
    if (needs_contents)
	ob = first_inventory();
    else
	ob = this_object();

    ob->drink_it();
}

