/* Do not remove the headers from this file! see /USAGE for more info. */

// Things which are common to all combat modules

#include <combat.h>

inherit __DIR__ "results";

int query_ghost();
object get_target();
void target_is_asleep();
void stop_attacking();
string query_weapon_damage_kind();
class combat_result array take_a_swing(object);
void check_condition(int);

void dispatch_opponent() {
    class combat_result array res = ({ new(class combat_result, 
					   special : RES_FATAL | RES_NO_RESISTANCE,
					   kind : query_weapon_damage_kind()) });
    
    res = negotiate_result(res);
    print_result(res);
    query_target()->do_damage(res);
}

void attack() {
    object target;
    mixed tmp;
    class combat_result array result;
    
    if (tmp = check_condition(1)) {
	if (tmp[<1] == '\n')
	    write(tmp);
	else
	    simple_action(tmp + " so $p blows are ineffective.");
	return;
    }
    if (query_ghost() || !(target = get_target())) {
	stop_attacking();
	return;
    }
    if (target->query_asleep()) {
	/* Our target is unconscious.  We get to have our way with them
	 * *evil grin*
	 */
	target_is_asleep();
	return;
    }
    result = take_a_swing(target);
    if (!result) return;
    
    result = negotiate_result(result);
    print_result(result);
    target->hit_living(result);
}
