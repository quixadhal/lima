/* Do not remove the headers from this file! see /USAGE for more info. */

// the traditional combat module; used when COMBAT_STYLE is COMBAT_TRADITIONAL
// in /include/config_combat.h

inherit __DIR__ "base";
inherit __DIR__ "base/hit_points";

#define PERCENT_PER_PENALTY		10

void refresh_stats() { 
    // players override this;
    // monsters can also use it to adjust things once per round
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

void do_damage(mixed res, string kind) {
    if (stringp(res)) {
	switch (res) {
	case "disarm":
	    unwield();
	    query_weapon()->move(environment());
	    break;
	case "fatal":
	case "dispatch":
	case "suicide":
	    die();
	    break;
	}
    } else
	reduce_hp(res);
}

void take_a_swing() {
    int them, us, chance, roll;
    object target;
    mixed result, dam;
    object weapon;

    if (query_ghost()) {
	attacking = 0;
	return;
    }

    if (!(target = get_target())) {
	attacking = 0;
	return;
    }

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
	    return;
	}
    }

    roll = random(100);
    if (roll > chance)
	dam = "miss";
    else if (roll == chance)
	dam = "disarm";
    else {
	dam = random(weapon->query_weapon_class() + damage_bonus()) + 1;
    }

    result = dam = negotiate_result(dam, weapon_type);

    if (intp(dam)) {
	if (dam == -1) return;

	them = target->query_hp();
	if (dam >= them) result = "fatal";
	else
	    switch (fuzzy_divide(result * 100, them)) {
	    case 0: result = "none"; break;
	    case 1: result = "dam1"; break;
	    case 2..3: result = "dam2"; break;
	    case 4..5: result = "dam3"; break;
	    case 6..9: result = "dam4"; break;
	    case 10..14: result = "dam5"; break;
	    case 15..24: result = "dam6"; break;
	    case 25..49: result = "dam7"; break;
	    case 50..79: result = "dam8"; break;
	    case 80..100: result = "dam9"; break;
	    default: result = "dam10";
	    }
    }

    print_result(result);
    target->hit_living(dam, weapon_type);
}

void attack() {
    receive("Hp: " + query_hp() + "\n");
    take_a_swing();
}
