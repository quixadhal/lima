/* Do not remove the headers from this file! see /USAGE for more info. */

// the simple combat module; used when COMBAT_STYLE is COMBAT_SIMPLE in
// /include/config_combat.h

#include <combat.h>
#include <classes.h>

inherit __DIR__ "base";

string default_message(int dam, int them) {
    if (dam <= 0) return "!none";
    if (dam == 1) return "!light";
    return "!serious";
}

private class combat_result make_result(int diff)
{
    class combat_result ret = new(class combat_result);
    int tmp;

    switch (tmp = (diff + random(10))) {
    case 5:
	ret->special |= RES_DISARM;
	break;
    case 4:
	ret->special |= RES_KNOCKDOWN;
	break;
    case 3:
	ret->damage = 1;
	ret->kind = weapon_type;
	break;
    case 2:
	ret->damage = 2;
	ret->kind = weapon_type;
	break;
    case 1:
	ret->special |= RES_KNOCKOUT;
	break;
    default:
	if (tmp < 0) {
	    ret->special |= RES_FATAL;
	    ret->message = "!fatal";
	} else {
	    ret->message = "!miss";
	}
    }
    // Not foolproof; but one can show there isn't a foolproof solution.
    // The target gets the last say on results, and could theoretically
    // choose to die from anything, even a miss.
    if (ret->damage >= query_target()->query_hp())
        ret->special |= RES_MIGHT_BE_FATAL;
    return ret;
}

void mudlib_setup() {
    ::mudlib_setup();
    
    /* self is an M_DAMAGE_SOURCE. initialize it. */
    set_combat_messages("combat-unarmed");
    //The bonus we get for fighting barehanded. Usually negative (weapons help)
    set_wield_bonus(-3);

    /* by pinging query_weapon(), we will default to self as a weapon */
    (void)query_weapon();
}

/* This is the big honking "take a swing" subroutine.  Normally,
it it called from heart_beat(), but can be called from elsewhere
to implement multiple attacks, etc.  Note that the fight must
already be started, though.  It takes a swing at the current
target. */

//:FUNCTION take_a_swing
//Take a swing at the person we are attacking
class combat_result array take_a_swing(object target)
{
    int diff;
    class combat_result array result;
    int hp;
    int them;
    int us;
    
    them = target->query_hp();
    hp = query_hp();

#ifdef DEBUG_COMBAT
    {
        int bn = query_weapon()->query_wield_bonus(target);
        int pen = query_penalty();
	string bstr = (bn ? sprintf("[%+i]", bn) : "");
        string pstr = (pen ? "[-" + pen + "]" : "");
        us = hp + query_weapon()->query_wield_bonus(target) - query_penalty();
        diff = us - them;
        receive_private_msg(sprintf("US: %i%s%s THEM: %i == %+i\n", hp, bstr, pstr, them, diff));
    }
#else
    us = hp + query_weapon()->query_wield_bonus(target) - query_penalty();
    diff = us - them;
#endif

    if (diff > 3)
	diff = 3;
    else if ( diff < -5 )
	diff = -5;

    if ( hp < 3 && random(5) <= diff) {
	/* Act intelligent and attempt to stay alive ... */
	switch (panic()) {
	case 2:
	    stop_fight();
	    /* falls through */
	case 1:
	    return 0;
	}
    }

    return ({ make_result(diff) });
}
