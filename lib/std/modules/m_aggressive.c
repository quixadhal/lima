/* Do not remove the headers from this file! see /USAGE for more info. */

// aggressive.c
// Inheritable for aggressive monsters
// Onyx@Red Dragon
// NOTE: Someone who understands Lima vanilla combat should edit the
// line I indicated to make this compat.

void add_hook(string tag, function hook);
void remove_hook(string tag, function hook);
void switch_to(object who);
void attack();
private object my_loc;
private int aggression_chance = 100 ; // default is always aggressive

private void handle_attack(object who) {
    if (random(100) < aggression_chance)

	//////////////////////////////////////////
	// CHANGE THIS LINE 
	// (to whatever codes it takes to start combat with who in Lima)
	//////////////////////////////////////////

	switch_to(who);
    attack();
    // start_attacking(who);
}

//:THIS FUNCTION SHOULD NOT BE CALLED OR OVERLOADED
// It checks to see which objects in the room are players and attacks them.
private void agro_func(object who) {
    object item;

    if (who == this_object())
	foreach(item in all_inventory(environment())) {
	if (item->is_player())
	    handle_attack(item);
    }
    else handle_attack(who);
}

//:THIS FUNCTION SHOULD NOT BE CALLED OR OVERRIDDEN
// It moves the "object_arrived" hook (which is associated with a room)
// when the aggressive monster moves
void i_moved() {
    if (my_loc) my_loc->remove_hook("object_arrived", (:agro_func:));

    my_loc = environment();

    my_loc->add_hook("object_arrived", (:agro_func:));
}

//:FUNCTION set_aggressive
// This function is used to set the aggression chance of a monster in the
// range 0 (never aggressive) to 100 (completely aggressive).
void set_aggressive(int a) {
    if (a > 0) {
	aggression_chance = a;
	add_hook("move", (: i_moved :) );
	return;
    }
    remove_hook("move", (: i_moved :) );
    aggression_chance = 0;
}

//:FUNCTION query_aggressive
// This function returns 0 for unaggressive monsters, or the chance of 
// aggression for aggressive monsters.
int query_agressive() {
    return aggression_chance;
}
