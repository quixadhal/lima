/* Do not remove the headers from this file! see /USAGE for more info. */

// the simple combat module; used when COMBAT_STYLE is COMBAT_SIMPLE in
// /include/config_combat.h

inherit __DIR__ "base";
inherit __DIR__ "base/hit_points";

// stunned/asleep should be generalized, and linked with player commands.
// possibly paralysis too.
static int stunned, asleep, chance;

/* use a string.  It's a memory efficient int variable */
private static string combat_tbl="\
4000000000\
5400000000\
1540000000\
2154000000\
6215400000\
3621540000\
3362154000\
3336215400\
3333621540";

private static string* result_to_class;

string *shared_var_hack() { return result_to_class; }

void create() {
    ::create();
    if (file_name() + ".c" == __FILE__)
	result_to_class = ({ "miss", "light", "serious", "fatal",
			     "disarm", "knockdown", "knockout",
			     "dispatch", "wakeup" });
    else
	result_to_class = __FILE__->shared_var_hack();
    
    set_combat_messages("combat-unarmed");
    //The bonus we get for fighting barehanded. Usually negative (weapons help)
    set_wield_bonus(-3);
    unwield(); // this actually wields us
}

int query_asleep() {
    return asleep;
}

string translate_result(int result) {
    if (result == -1) return 0;
    return result_to_class[result];
}

//:FUNCTION target_is_asleep
//Called with the person we are attacking is asleep or unconscious.  Default
//behavior is to finish them off.

/* Override this function if you want your monster to do something other
 * than killing unconscious victims.  Rob them, etc ...
 */
void target_is_asleep()
{
    mixed res = negotiate_result("dispatch", weapon_type);
    print_result(res);
    query_target()->do_damage(res, weapon_type);
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

/* This is actually a misnomer.  It does the result of a round.
 * This is the function to call if you want to do damage to a
 * player (from a spell, etc).
 */
//:FUNCTION do_damage
//Apply a combat result to us.
void do_damage(mixed res, string kind) {
    switch(res) {
    case "disarm":
	unwield();
	query_weapon()->move(environment());
	break;
    case "fatal":
    case "dispatch":
    case "suicide":
	die();
	break;
    case "light":
	reduce_hp(1);
        break;
    case "serious":
        reduce_hp(2);
	break;
    case "knockdown":
	stun();
	break;
    case "knockout":
	knock_out();
	break;
    }
}

/* This is the big honking "take a swing" subroutine.  Normally,
it it called from heart_beat(), but can be called from elsewhere
to implement multiple attacks, etc.  Note that the fight must
already be started, though.  It takes a swing at the current
target. */

//:FUNCTION take_a_swing
//Take a swing at the person we are attacking
void take_a_swing() {
    object target;
    int them, us;
    int diff;
    int result;
    int hp;
    string tmp;
    
    /* check our status */
    if (query_ghost()) {
	attacking = 0;
	return;
    }
    if (stunned || asleep) return;

    /* This checks if they're dead or the fight is over, etc */
    if (!(target = get_target())) {
	attacking = 0;
	return;
    }
    
    them=target->query_hp();
    if (target->query_asleep()) {
//### asleep needs a lot of work; currently it doesn't prevent commands etc
	/* Our target is unconscious.  We get to have our way with them
	 * *evil grin*
	 */
	target_is_asleep();
	return;
    }
    hp = query_hp();
#ifdef DEBUG_COMBAT
    {
        int bn = query_weapon()->query_wield_bonus(target);
        int pen = query_penalty();
	string bstr = (bn ? sprintf("[%+i]", bn) : "");
        string pstr = (pen ? "[-" + pen + "]" : "");
        us = hp + query_weapon()->query_wield_bonus(target) - query_penalty();
        diff = us - them;
        tell_object(this_object(), sprintf("US: %i%s%s THEM: %i == %+i\n", hp, bstr, pstr, them, diff));
    }
#else
    us = hp + query_weapon()->query_wield_bonus(target) - query_penalty();
    diff = us - them;
#endif
    if (diff>3) diff=3;
    else if (diff<-5) diff=-5;

    if (hp<3 && random(5)<=diff) {
	/* Act intelligent and attempt to stay alive ... */
	switch (panic()) {
	case 2:
	    stop_fight();
	    /* falls through */
	case 1:
	    return;
	}
    }
    result = combat_tbl[(diff+5)*10+random(10)]-'0';
    if (result < 3 && result>=them) result = 3;

    tmp = negotiate_result(translate_result(result), weapon_type);
    print_result(tmp);
    target->hit_living(tmp, weapon_type);
}

void attack() {
    if (stunned) {
	if (stunned > time()) {
	    simple_action("$N $vare still recovering from that last blow, so $p blows are ineffective.\n");
	    return;
	}
	stunned = 0;
    }
    if (asleep) {
//###FIXME: not right for players
	if (random(5)<=chance++) {
	    wake_up();
	    print_result("wakeup");
	}
	return;
    }
    take_a_swing();
}
