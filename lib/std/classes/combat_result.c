/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * 'special' is a special effect, like 'disarm', or 'fatal'
 * 'damage' is just an ammount of damage; 'kind' is the kind
 * 'message' is the message used; it is from the attacker's point of view,
 *     with $o0 being the attackers weapon, and $o1 being the defender's.
 */
class combat_result {
    int special;
    int damage;
    string kind;
    string message;
}
