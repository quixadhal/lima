/* Do not remove the headers from this file! see /USAGE for more info. */

#include "combat.h"
#include "mudlib.h"

inherit LIVING;

/* believe it or not, a monster is a weapon.  Unarmed monsters
 * wield themselves.
 * This makes handling of hit functions, etc for monsters
 * MUCH easier.
 */
inherit M_WIELDABLE;

mixed ob_state() {
    /* ob_state too.  Techinically, these are unecessary, but we'll be safe. */
    return wieldable::ob_state();
}

/* Most of the combat code is in here.  Much of what is in here
 * would traditionally be in /std/living, but that's stupid since not all
 * living things need combat code (well, maybe on a hack and slash...)
 *
 * -Beek
 * 
 * P.S. It's well commented so you can learn how to use it.  Do me
	  a favor and comment your code too :) 
 */

/* Globals */
private int hp = 1;
private int max_hp = 1;
private static int hb_time = time();
int heal_rate = 10;

static object weapon;
string weapon_type = "blow";
private static int weapon_bonus;

static int stunned, chance;
static object target;
static object op_weapon;
static object* combat_who = ({});
static object* other_targets = ({});
static object* armors = ({});

/* use a string.  It's a memory efficient int variable */
private static string combat_tbl="\
1540000000\
2154000000\
6215400000\
3621540000\
3362154000\
3336215400\
3333621540";
private static string* result_to_class = ({});

void hb_off();
void unwield();
object get_target();
void panic();
varargs void stop_fight(object);
void heal_us(int);

create()
{
    living::create();
    result_to_class = ({ "miss", "light", "serious", "fatal",
      "disarm", "knockdown", "knockout", "dispatch", "wakeup" });
    hb_off();
    unwield();
}

//:FUNCTION set_max_hp
//Set the maximum number of hit points of a monster, and also set it's
//hit points to the new max
void set_max_hp(int x) {
    hp = max_hp = x;
}

//:FUNCTION set_hp
//Set the hit points of a monster
//see: set_max_hp
void set_hp(int x) {
    hp = x;
}

//:FUNCTION print_result
//Print the result of a round of combat
void print_result( int res ) {
    string *msgs;
    string message;

    if ( !weapon )
	weapon = this_object();
    message = weapon->query_msg(result_to_class[res]);

    if (!message)
    {
	simple_action("$N $vare puzzled because $n $vhave no message for '"+result_to_class[res]+"'.\n");
    }
    else
    {
	msgs = action( combat_who, message, weapon, op_weapon);
	inform( combat_who, msgs, environment() );
    }
}

//:FUNCTION target_is_asleep
//Called with the person we are attacking is asleep or unconscious.  Default
//behavior is to finish them off.

/* Override this function if you want your monster to do something other
 * than killing unconscious victims.  Rob them, etc ...
 */
void target_is_asleep()
{
    print_result(target->do_damage(RES_DISPATCH,"blow"));
}

/* This is the big honking "take a swing" subroutine.  Normally,
it it called from heart_beat(), but can be called from elsewhere
to implement multiple attacks, etc.  Note that the fight must
already be started, though.  It takes a swing at the current
target. */

//:FUNCTION attack
//Take a swing at the person we are attacking
void attack() {
    int them, us;
    int diff;
    int result;
    /* check our status */
    if (stunned || hp<=0) return;
    /* This checks if they're dead or the fight is over, etc */
    if (!get_target()) {
	hb_off();
	return;
    }
    if ((them=target->query_hp())<0) {
	/* Our target is unconscious.  We get to have our way with them
	 * *evil grin*
	 */
	target_is_asleep();
	return;
    }
    us = hp + weapon_bonus;
    diff = us - them;
    if (diff>3) diff=3;
    if (diff<-3) diff=-3;

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
    result = combat_tbl[(diff+3)*10+random(10)]-'0';
    if (result <3 && result>=them) result = 3;

    /* The weapon gets a chance to fiddle with the result if it
     * wants to. */
    if ( !weapon )
	weapon = this_object();
    result = weapon->adjust_result(result);
    result = target->hit_living(result, weapon_type);
    print_result(result);
}

/* Pretend that our heartbeat has been on since hb_time, even though
 * it hasn't ... 
 */
//:FUNCTION query_hp
//Find the current number of hitpoints of a monster
int query_hp() {
    if (hb_time) {
	heal_us((time()-hb_time)/heal_rate);
	hb_time = time();
    }
    return hp;
}

//:FUNCTION query_max_hp
//Find the maximum number of hitpoints of a monster
int query_max_hp()
{
    return max_hp;
}

//:FUNCTION query_ghost
//return 1 if the monster is dead
int query_ghost() {
    if (hp==0) return 1;
}

void restart_heart();
void switch_to(object);
void set_up_target();

/* Call this function to make us start a fight with "who".  It's
 * ok if we're already fighting them.  If they aren't the current
 * attacker, then they will be.  We also take a swing, for good
 * measure :)
 */
//:FUNCTION start_fight
//Add someone to the list of people we are attacking.  If we were already
//attacking them, make them the primary person we are attacking.  Then
//take a swing at them.
int start_fight(object who) {
    if (!(who->attackable())) return 0;
    if (who == target) return 1;
    if (hb_time) restart_heart();

    if (member_array(who, other_targets)) {
	switch_to(who);
    } else {
	if (target) other_targets+= ({ target });
	target = who;
    }

    set_up_target();
    return 1;
}

//:FUNCTION attackable
//return 1 if we can be attacked.
int attackable() {
    return 1;
}

/* Find someone to attack.  Return zero if we're dead or asleep or
 * have noone to attack.
 */
//:FUNCTION get_target
//Get someone to attack from our attackers list
object get_target() {
    if (hp<=0) return 0;
    while ((!target || target->query_ghost() ||
	environment() != environment(target)) && sizeof(other_targets)) {
	target = other_targets[0];
	other_targets = other_targets[1..<0];
    }
    if (target)
	if (target->query_ghost() || environment() != environment(target))
	    target = 0;
    if (target) set_up_target();
    return target;
}

void flee();
void surrender();

//:FUNCTION panic
//Do something intelligent when we are about to die
/* Override this function to have your monster do something else when it's
 * hp's get low. */
void panic() {
    if (random(10)<3) flee();
    else surrender();
}

//:FUNCTION adjust_result
//modify a combat result
int adjust_result(int res)
{
    return res;
}

//:FUNCTION query_wield_bonus
//The bonus we get for fighting barehanded.  Usually negative (weapons help)
int query_wield_bonus()
{
    /* The default is for monsters not to fight well bare-handed. */
    return -3;
}

void stun();
void die();
void knock_out();

/* This is actually a misnomer.  It does the result of a round.
 * This is the function to call if you want to do damage to a
 * player (from a spell, etc).  Note that RES_1_HP == 1 and 
 * RES_2_HP = 2, so you can just pass the damage.  Note that
 * '2' is a serious wound, and you shouldn't do more damage
 * than that.  3 == RES_KILL.  Use it to zap a player.
 */
//:FUNCTION do_damage
//Apply a combat result to us.
int do_damage(int res, string kind) {
    int i;
    stunned = 0;
    for (i=0; i<sizeof(armors); i++) {
	res = armors[i]->adjust_result(res,kind);
    }
    switch(res) {
    case RES_DISARM:
	if (weapon == this_object()) {
	    stun();
	    return RES_KNOCKDOWN;
	}
	else unwield();
	break;
    case RES_FATAL:
    case RES_DISPATCH:
	die();
	break;
    case RES_1_HP:
    case RES_2_HP:
	hp -= res;
	break;
    case RES_KNOCKDOWN:
	stun();
	break;
    case RES_KNOCKOUT:
	knock_out();
	break;
    }
    return res;
}

/* Be careful how you call this one.  It attacks the caller, which
 * might not be what you wan.
 */
//:FUNCTION hit_living
//Do some damage to a monster.  The monster then attacks previous_object()
int hit_living(int res, string kind) {
    res = do_damage(res,kind);
    if (!target) start_fight(previous_object());
    return res;
}

//:FUNCTION switch_to
//Make the specified target the primary target
void switch_to(object who) {
    other_targets += ({ target });
    target = who;
    other_targets -= ({ who });
    set_up_target();
}

//:FUNCTION set_up_target
//Set up some internal data structures which need to be changed when we
//change targets
void set_up_target()
{
    if ( !weapon )
	weapon = this_object();
    weapon_bonus = weapon->query_wield_bonus(target);
    op_weapon = target->query_weapon();
    combat_who = ({ this_object(), target });
}

//:FUNCTION query_weapon
//return the weapon we are using.  The weapon will be us if we are unarmed.
object query_weapon()
{
    return weapon;
}


/* Override flee() and surrender() to define or modify the behaviour
 * of your monster.  Return 1 of the monster shouldn't take a swing.
 * Return 2 if the monster should stop attacking. (useful for
 * surrender() */
//:FUNCTION flee
//Try to run away.
void flee() {
}

//:FUNCTION flee
//Try to run surrender
void surrender() {
}

//:FUNCTION stop_hitting_me
//remove the previous object from the target list
void stop_hitting_me()
{
    if (previous_object()==target)
	target = 0;
    else
	other_targets -= ({ previous_object() });
}

/* This stops attacking "who".  It also takes care of stoping them
 * from attacking us.  stop_fight() or stop_fight(0) stops all
 * combat.
 */
//:FUNCTION stop_fight
//stop fighting with object ob (handles making sure they stop fighting
//with us too).  stop_fight(0) stops all fights.
varargs void stop_fight(object who)
{
    if (!who)
    {
	target->stop_hitting_me();
	other_targets->stop_hitting_me();
	target = 0;
	other_targets = ({});
	return;
    }

    if (who==target)
	target = 0;
    else
	other_targets -= ({ who });
    who->stop_hitting_me();
}

/* Call the following to make your monster do the appropriate thing */

//:FUNCTION wield
//wield the specified object
void wield(object ob)
{
    weapon = ob;
    weapon->mark_wielded_by(this_object());
    weapon_type = weapon->query_wield_type();

    /*
    ** If we are already fighting, then set the bonus based on who we
    ** are fighting.
    */
    if ( target )
	weapon_bonus = weapon->query_wield_bonus(target);
}

//:FUNCTION do_wield
//wield the specified object, as well as checking if it is valid and printing
//the appropriate message
int do_wield(object ob) {
    if (!(ob->valid_wield())) return 0;
    simple_action(ob->query_wield_message(), ob);
    wield(ob);
    return 1;
}

//:FUNCTION unwield
//stop wielding the weapon we were wielding
void unwield() {
    if (weapon)
	weapon->mark_wielded_by(0);
    wield(this_object());
}

//:FUNCTION vaporize
//remove the corpse in a puff of black smoke
void vaporize() {
    simple_action("Soon after $n $vhave breathed $p last, $p body disappears in a puff of black smoke.\n");
    destruct(this_object());
}

//:FUNCTION die
//Kick the bucket.
void die() {
    hp = 0;
    call_out("vaporize", 1);
}

//:FUNCTION stun
//Stun us
void stun() {
    stunned = 1;
}

//:FUNCTION wake_up
//Wake up after being asleep or stunned
void wake_up() {
    if (hp<0) hp = -hp;
}

//:FUNCTION knock_out
//Knock us out
void knock_out() {
    if (hp>0) hp = -hp;
    chance = 0;
}

void heart_beat() {
    if (stunned) {
	simple_action("$N $vare still recovering from that last blow, so $p blow are ineffective.\n");
	stunned = 0;
	return;
    }
    if (hp<0) {
	if (random(5)<=chance++) {
	    wake_up();
	    print_result(RES_WAKE_UP);
	}
	return;
    }
    attack();
}

//:FUNCTION hb_off
//Turn our heartbeat off, and save the time for updating things later
void hb_off() {
    set_heart_beat(0);
    hb_time = time();
}

//:FUNCTION restart_heart
//Turn our heart back on, and do any healing which should have occured
void restart_heart() {
    heal_us((time() - hb_time)/heal_rate);
    set_heart_beat(1);
}

//:FUNCTION heal_us
//Heal us a specified amount, truncating at max_hp
void heal_us(int amt) {
    hp += amt;
    if (hp>max_hp) hp = max_hp;
}

//:FUNCTION wear_ob
//start wearing a piece of armor
/* if set_worn() returns 1, the armor wants to modify hits */
void wear_ob(object ob) {
    if (ob->set_worn(this_object()))
	armors += ({ ob });
}

//:FUNCTION remove_ob
//stop wearing a piece of armor
void remove_ob(object ob) {
    if (ob->set_worn(0))
	armors -= ({ ob });
}

