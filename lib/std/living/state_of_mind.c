/* Do not remove the headers from this file! see /USAGE for more info. */

//### This stuff needs to be linked to affect commands.
private static int stunned, asleep, chance;

int query_ghost() {
    return 0;
}

int query_asleep() {
    return asleep;
}

int query_stunned() {
    return stunned;
}

/* Call the following to make your monster do the appropriate thing */

//:FUNCTION stun
//Stun us
void stun() {
    stunned = time() + 5;
}

//:FUNCTION wake_up
//Wake up after being asleep or knocked out
void wake_up() {
    if (asleep) {
	asleep = 0;
	stunned = time() + 5;
    }
}

//:FUNCTION knock_out
//Knock us out
void knock_out() {
    asleep = 1;
    chance = 0;
}

// Warning: Elsewhere we rely on the fact that if urgent == 0, nothing
// is printed, and a complete sentence is returned.
mixed check_condition(int urgent) {
    if (query_ghost()) {
	stunned = 0;
	asleep = 0;
	return 0;
    }
    
    if (urgent && stunned > time())
	return "$N $vare still recovering from that last blow, ";

    if (asleep) {
	if (urgent) {
	    if (random(5)<=chance++) {
		wake_up();
//FIXME: print_result takes a class combat_result array now.
#if 0
		print_result("wakeup");
#endif
	    } else
		return "You try desparately to wake up, but fail.\n";
	} else {
	    return "But you are asleep!\n";
	}
    }
    return 0;
}
