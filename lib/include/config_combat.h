/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef CONFIG_COMBAT_H
#define CONFIG_COMBAT_H

// Noisy debugging info while fighting?
#undef DEBUG_COMBAT

// Zorkish
#define COMBAT_SIMPLE		1
// 2.4.5-ish
#define COMBAT_TRADITIONAL	2
// complex :-)
#define COMBAT_COMPLEX		3

// NOTE: Changing the following may cause existing players to lose/have
//       incorrect stats.
#define COMBAT_STYLE		COMBAT_SIMPLE

// Either we attack the same person over and over, and keep track of who
// else we are hostile to, or we pick a random person.
#define TARGETTING_IS_RANDOM

// If you don't define this, monsters disappear in a puff of smoke, ala Zork
#undef DEATH_USES_CORPSES

// Does combat use heart_beats, or is it up to the player when to attack?
#define COMBAT_USES_HEARTBEATS

// If this is defined, a message is sent to everyone logged on whenever
// someone dies.
#define DEATH_MESSAGES

#endif
