/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat.h>

// the traditional combat module; used when COMBAT_STYLE is COMBAT_TRADITIONAL
// in /include/config_combat.h

inherit __DIR__ "base";

#define PERCENT_PER_PENALTY		10

void refresh_stats() { 
    // players override this;
    // monsters can also use it to adjust things once per round
}

string default_message(int dam, int them) {
    switch (fuzzy_divide(dam * 100, them)) {
    case 0: return "!none";
    case 1: return "!dam1";
    case 2..3: return "!dam2";
    case 4..5: return "!dam3";
    case 6..9: return "!dam4";
    case 10..14: return "!dam5";
    case 15..24: return "!dam6";
    case 25..49: return "!dam7";
    case 50..79: return "!dam8";
    case 80..100: return "!dam9";
    default: return "!dam10";
    }
}

void create() {
    ::create();

    set_combat_messages("combat-unarmed");
    set_wield_bonus(-25); // -25% to hit bare hand.
    set_weapon_class(3);  // and low WC
    unwield();
}

// there should be an interface to these for monsters, but for now
// just call set_weapon_class() and set_wield_bonus()
int to_hit_base() {
    return 50;
}

int hit_skill() {
    return 0;
}

int damage_bonus() {
    return 0;
}

class combat_result array
take_a_swing(object target) {
    int them, us, chance, roll;
    class combat_result res;
    object weapon;

    refresh_stats();

    weapon = query_weapon();

#ifdef DEBUG_COMBAT
    {
	int thb = target->to_hit_base();
	int bn = weapon->query_wield_bonus(target);
	int hs = hit_skill();
	int pen = query_penalty();
	string bstr = (bn ? sprintf("[%+i]", bn) : "");
	string pstr = (pen ? "[-" + (pen*PERCENT_PER_PENALTY) + "]" : "");

	chance = thb + bn + hs - pen*PERCENT_PER_PENALTY;
	tell_object(this_object(), sprintf("THB: %i%s HS: %i%s == %i\n",
			thb, bstr, hs, pstr, chance));
    }
#else
    chance = target->to_hit_base() + weapon->query_wield_bonus(target)
	     + hit_skill() - query_penalty()*PERCENT_PER_PENALTY;
#endif
    
    if (chance < 2) chance = 2;
    else if (chance > 98) chance = 98;

    if (query_hp() < query_max_hp()/5 && random(50) < chance) {
	switch (panic()) {
	case 2:
	    stop_fight();
	    /* falls through */
	case 1:
	    return 0;
	}
    }

    res = new(class combat_result);

    roll = random(100);
    if (roll > chance)
	res->message = "!miss";
    else if (roll == chance)
	res->special |= RES_DISARM;
    else {
	res->damage = random(weapon->query_weapon_class() + damage_bonus()) + 1;
	// Not foolproof; but one can show there isn't a foolproof solution.
        // The target gets the last say on results, and could theoretically
        // choose to die from anything, even a miss.
        if (res->damage >= query_target()->query_hp())
            res->special |= RES_MIGHT_BE_FATAL;
	res->kind = weapon_type;
    }
    
    return ({ res });
}

void attack() {
    tell_object(this_object(), "Hp: " + query_hp() + "\n");
    ::attack();
}
