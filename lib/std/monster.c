/* Do not remove the headers from this file! see /USAGE for more info. */

/* Most of the combat code is in here.  Much of what is in here
 * would traditionally be in /std/living, but that's stupid since not all
 * living things need combat code (well, maybe on a hack and slash...)
 *
 * -Beek
 */

#include "combat.h"

#if COMBAT_STYLE == COMBAT_SIMPLE
inherit __DIR__ "monster/simple";
#elif COMBAT_STYLE == COMBAT_TRADITIONAL
inherit __DIR__ "monster/traditional";
#elif COMBAT_STYLE == COMBAT_COMPLEX
inherit __DIR__ "monster/complex";
#else
#error Improperly specified COMBAT_STYLE in /include/config_combat.h
#endif

static object* armors = ({});

//:FUNCTION start_fight
//Add someone to the list of people we are attacking.  If we were already
//attacking them, make them the primary person we are attacking.  Then
//take a swing at them.
int start_fight(object who) {
    if (!(who->attackable())) return 0;
    attacked_by(who, 1);
    return 1;
}

//:FUNCTION attackable
//return 1 if we can be attacked.
int attackable() {
    return 1;
}

void flee();
void surrender();

//:FUNCTION adjust_result
//modify a combat result we inflict
mixed adjust_my_result(mixed res, string type)
{
    return res;
}

//:FUNCTION adjust_result
//modify a combat result inflicted on us
mixed adjust_result(mixed res, string type)
{
    object armor;
    
    armors -= ({ 0 });
    foreach (armor in armors)
	res = armor->adjust_result(res,type, previous_object());

    if (res == "disarm") {
	object weapon = query_weapon();
	if (weapon == this_object() || weapon->prevent_disarm())
	    return "knockdown";
    }
    
    return res;
}

void commit_suicide() {
    mixed res;

    switch_to(this_object());
    res = negotiate_result("suicide", weapon_type);
    print_result(res);
    do_damage(res, weapon_type);
    // just in case the result was modified
    stop_fight(this_object());
}

/* Be careful how you call this one.  It attacks the caller, which
 * might not be what you want.
 */
//:FUNCTION hit_living
//Do some damage to a monster.  The monster then attacks previous_object()
void hit_living(mixed res, string kind) {
    do_damage(res, kind);
    attacked_by(previous_object());
}

//:FUNCTION add_armor
//start wearing a piece of armor
void add_armor(object ob) {
    armors += ({ ob });
}

//:FUNCTION remove_armor
//stop wearing a piece of armor
void remove_armor(object ob) {
    armors -= ({ ob });
}

mixed direct_kill_liv(object ob) {
    return 1;
}

mixed direct_kill_liv_with_ob(object ob) {
    return 1;
}

