/* Do not remove the headers from this file! see /USAGE for more info. */

/* Most of the combat code is in here.  Much of what is in here
 * would traditionally be in /std/living, but that's stupid since not all
 * living things need combat code (well, maybe on a hack and slash...)
 *
 * -Beek
 */

#include "combat.h"

inherit LIVING;
inherit M_DAMAGE_SOURCE;

inherit __DIR__ "monster/behaviors";
inherit __DIR__ "monster/death";
inherit __DIR__ "monster/hit_points";
inherit __DIR__ "monster/target";
inherit __DIR__ "monster/wield";

#ifdef USE_BODYSLOTS
inherit __DIR__ "monster/bodyslots";
#endif

#ifdef COMBAT_USES_HEARTBEATS
inherit __DIR__ "monster/heart_beat";
#else
inherit __DIR__ "monster/non_heart_beat";
#endif

#if COMBAT_STYLE == COMBAT_SIMPLE
# define COMBAT_MODULE simple
# define S_COMBAT_MODULE "simple"
#elif COMBAT_STYLE == COMBAT_TRADITIONAL
# define COMBAT_MODULE traditional
# define S_COMBAT_MODULE "traditional"
#elif COMBAT_STYLE == COMBAT_COMPLEX
# error COMBAT_COMPLEX not written yet
# define COMBAT_MODULE complex
# define S_COMBAT_MODULE "complex"
#endif

#ifndef COMBAT_MODULE
#error Improperly specified COMBAT_STYLE in /include/config_combat.h
#endif


inherit __DIR__ "monster/combat/" S_COMBAT_MODULE ;

#ifdef USE_BODYSLOTS
object get_random_clothing();
#else
static object* armors = ({ });
#endif


//:FUNCTION start_fight
//Add someone to the list of people we are attacking.  If we were already
//attacking them, make them the primary person we are attacking.  Then
//take a swing at them.
int start_fight(object who)
{
    if (!(who->attackable()))
	return 0;
    attacked_by(who, 1);
    return 1;
}

//:FUNCTION attackable
//return 1 if we can be attacked.
int attackable()
{
    return 1;
}

//:FUNCTION adjust_my_result
//modify a combat result we inflict
class combat_result array adjust_my_result(class combat_result array res)
{
    return res;
}

void intrinsic_resistance(class combat_result res)
{
}

//:FUNCTION adjust_result
//modify a combat result inflicted on us
nomask mixed adjust_result(mixed res)
{
    int hp = query_hp();
    

#ifdef USE_BODYSLOTS
    object armor = get_random_clothing();
    if(armor)
        armor->adjust_result(res, previous_object());
#else
    armors -= ({ 0 });
    foreach (object armor in armors)
	res = armor->adjust_result(res, previous_object());
#endif

    if (arrayp(res))
	foreach (class combat_result result in res) {
	    intrinsic_resistance(result);

	    if (result->special & RES_DISARM) {
		object weapon = query_weapon();
		if (weapon == this_object() || weapon->prevent_disarm())
		    result->special &= ~RES_DISARM;
	    }
	    
	    if (hp > 0) {
		if (hp <= result->damage)
		    result->special |= RES_FATAL;
		hp -= result->damage;
	    } else {
		result->special |= RES_NONE;
	    }
	}

    return res;
}

void commit_suicide()
{
    class combat_result res = new(class combat_result, 
				  special : RES_NO_RESISTANCE | RES_FATAL,
				  message : "!suicide");
    mixed result;
    
    switch_to(this_object());
    result = negotiate_result( ({ res }) );
    print_result(result);
    do_damage(result);
    // just in case the result was modified
    stop_fight(this_object());
}

/* Be careful how you call this one.  It attacks the caller, which
 * might not be what you want.
 */
//:FUNCTION hit_living
//Do some damage to a monster.  The monster then attacks previous_object()
void hit_living(class combat_result array res)
{
    do_damage(res);
    attacked_by(previous_object());
}

#ifndef USE_BODYSLOTS
//:FUNCTION add_armor
//start wearing a piece of armor
void add_armor(object ob)
{
    armors += ({ ob });
}

//:FUNCTION remove_armor
//stop wearing a piece of armor
void remove_armor(object ob)
{
    armors -= ({ ob });
}
#endif
mixed direct_kill_liv(object ob)
{

    if(ob->query_ghost())
     this_body()->simple_action("$N $vtry to attack a ghost.");
    return 1;
}

mixed direct_kill_liv_with_obj(object ob)
{
    return 1;
}

void mudlib_setup()
{
  ::mudlib_setup();
  COMBAT_MODULE::mudlib_setup();
  set_max_capacity(VERY_LARGE*3);
}

mapping lpscript_attributes() {
    return ( living::lpscript_attributes() +
	     m_damage_source::lpscript_attributes() +
	     ([
		 "max_hp" : ({ LPSCRIPT_INT, "setup", "set_max_hp" }),
		 ])
	     );
}

void remove() {
#ifndef COMBAT_USES_HEARTBEATS
    non_heart_beat::remove();
#endif
    m_damage_source::remove();
    living::remove();
}

int query_ghost()
{
    return hit_points::query_ghost();
}

string diagnose() {
    string ret;
    
    if (query_ghost())
	return "$N $vare dead.  Other than that, things are going well for $n.\n";

    if (query_asleep())
	ret = "$N $vare asleep.\n";
    else if (query_stunned())
	ret = "$N $vare stunned.\n";
    else
	ret = "";

    switch (query_hp() * 100 / query_max_hp()) {
    case 0..10:
	return ret + "$N $vare about to die.\n";
    case 11..25:
	return ret + "$N $vare in very bad shape.\n";
    case 26..50:
	return ret + "$N $vare badly wounded.\n";
    case 51..75:
	return ret + "$N $vare wounded.\n";
    case 76..85:
	return ret + "$N $vare lightly wounded.\n";
    case 86..95:
	return ret + "$N $vare a bit hurt, but nothing serious.\n";
    case 96..99:
	return ret + "$N $vare a bit roughed up.\n";
    case 100:
	return ret + "$N $vare in perfect health.\n";
    default:
	error("query_hp() out of bounds.\n");
    }
}
