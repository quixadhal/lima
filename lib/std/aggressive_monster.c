/* Do not remove the headers from this file! see /USAGE for more info. */

// aggressive_monster.c - A monster that will attack others when they move
//                        into the room or when the monster moves into
//                        their room.
// Iizuka: May 13, 1998.

inherit ADVERSARY;
inherit M_AGGRESSIVE;

void mudlib_setup()
{
   adversary::mudlib_setup();
   m_aggressive::mudlib_setup();
}

/*
 * For specialized effects like racial biases, try overloading
 * handle_attack(). e.g.:
 *
 * private void handle_attack(object who)
 * {
 *    if(who->query_race() == "human")
 *       ::handle_attack(who);
 * }
 *
 */
