/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * COMBAT_MODULES_H: Beek, April 7, 1997.
 * Major changes by Iizuka: April 4, 1998.
 */

/* 
** DESCRIPTION OF OPTIONS
**
*****************************************************************************
**                                                                         **
**                          TO CORPSE OR NOT TO CORPSE                     **
**                                                                         **
** DEATH_VAPORIZE Select this option if you want no corpses to             **
**                be left when something dies.                             **
** DEATH_CORPSES  Corpses are left behind when something dies.             **
**                                                                         **
*****************************************************************************
**
*****************************************************************************
**                                                                         **
**                                   HEALTH                                **
**                                                                         **
** HEALTH_HITPOINTS		This is the more traditional way of handling         **
**                    health.  The entire body has a set number of         **
**                    hitpoints.                                           **
** HEALTH_LIMBS       This type of health is based on the limbs            **
**                    that a body has.  There can be vital limbs           **
**                    which, when their hitpoints drop to 0, the           **
**                    body dies. Normal limbs are disabled then.           **
** HEALTH_WOUNDS      A more complex limb based system than                **
**                    HEALTH_LIMBS, where wounds can be accumulated        **
**                                                                         **
*****************************************************************************
** 
*****************************************************************************
**                                                                         **
**                           WEILDING WEAPONS                              **
**                                                                         **
** WIELD_SIMPLE 		Only one weapon can be wielded at a time.              **
** WIELD_MULTIPLE   Allow adversaries to wield multiple weapons            **
**                  without requiring a limb-based health module           **
** WIELD_LIMBS			Any 'wielding' limb can wield a weapon.                **
**                  This module requires HEALTH_LIMBS or HEALTH_WOUNDS     **
**                                                                         **
*****************************************************************************
**
*****************************************************************************
**                                                                         **
**                             WEARING ARMOUR                              **
**                                                                         **
** ARMOR_SIMPLE_SLOTS	This is a simplistic way of determining            **
**                      where armour can be worn                           **
** ARMOR_COMPLEX_SLOTS  A more complicated slot method                     **
** ARMOR_LIMBS          Armour works with limbs.  Each limb that is        **
**                      marked able to bear armour, works properly.        **
**                      Requires HEALTH_LIMBS or HEALTH_WOUNDS             **
**                                                                         **
*****************************************************************************
**
*****************************************************************************
**                                                                         **
**                                    PULSE                                **
**                                                                         **
** PULSE_HEART_BEAT	    Does heart_beat() drive your combat?           **
** PULSE_NON_HEART_BEAT Or not?                                            **
**                                                                         **
*****************************************************************************
**
*****************************************************************************
**                                                                         **
**                                    BLOWS                                **
**                                                                         **
** BLOW_SIMPLE Damage is damage.  Who cares how you get it.                **
** BLOW_TYPES  Ok, so there really /is/ different kinds of  damage         **
**                                                                         **
*****************************************************************************
**
*****************************************************************************
**                                                                         **
**                                  FORMULAS                               **
**                                                                         **
** FORUMLA_SIMPLE  A simpler combat formula scheme (level-based)           **
** FORMULA_STATS   All combat calculations are based upon bodystats        **
** FORMULA_SKILLS  Uses Lima's akills system                               **
**                                                                         **
*****************************************************************************
**
*****************************************************************************
**                                                                         **
**                                 ADVANCEMENT                             **
**                                                                         **
** ADVANCEMENT_SIMPLE A simple experience based advancement.               **
**                    Presently this is all that comes with Lima.          **
**                                                                         **
****************************************************************************/

#ifndef COMBAT_MODULES_H
#define COMBAT_MODULES_H

/*
 * Nothing in here should be changed.
 */
/* internal use */
#define STR(y)       #y
#define STRINGIZE(z) STR(z)
#define MODULE(x, y) SUBDIR x "/" STR(y)

#define DEATH_VAPORIZE        1
#define DEATH_CORPSES         2
#define HEALTH_HITPOINTS      1
#define HEALTH_LIMBS          2
#define HEALTH_WOUNDS         3
#define WIELD_SINGLE          1
#define WIELD_MULTIPLE        2
#define WIELD_LIMBS           3
#define ARMOR_SIMPLE_SLOTS    1
#define ARMOR_COMPLEX_SLOTS   2
#define ARMOR_LIMBS           3
#define PULSE_HEART_BEAT      1
#define PULSE_NON_HEART_BEAT  2
#define BLOW_SIMPLE           1
#define BLOW_TYPES            2
#define FORMULA_SIMPLE        1
#define FORMULA_STATS         2
#define FORMULA_SKILLS        3
#define ADVANCEMENT_SIMPLE    1

/*
** Customize the following options, but be aware that
** changing some of them later on can, and probably will,
** screw up your user data files.
*/
#define DEATH_STYLE       DEATH_VAPORIZE
#define HEALTH_STYLE      HEALTH_HITPOINTS
#define WIELD_STYLE       WIELD_SINGLE
#define ARMOR_STYLE       ARMOR_COMPLEX_SLOTS
#define PULSE_STYLE       PULSE_HEART_BEAT
#define BLOW_STYLE        BLOW_TYPES
#define FORMULA_STYLE     FORMULA_SKILLS
#define ADVANCEMENT_STYLE ADVANCEMENT_SIMPLE

/* #define or #undef these. Modify them without fear of messing anything
   up in your .o files. */
#undef TARGETTING_IS_RANDOM
#undef DEBUG_COMBAT
#undef DEATH_MESSAGES

/* The maximum number of attacks an adversary can ever have. Again, this
   won't mess up your .o files either. */
#define MAX_ATTACK_SPEED   10

/* internal use. Don't edit below here unless you really
   know what you're doing. */
#if DEATH_STYLE == DEATH_CORPSES
#define DEATH_MODULE corpses
#else
#if DEATH_STYLE == DEATH_VAPORIZE
#define DEATH_MODULE vaporize
#endif
#endif

#if HEALTH_STYLE == HEALTH_HITPOINTS
#define HEALTH_MODULE hit_points
#else
#if HEALTH_STYLE == HEALTH_LIMBS
#define HEALTH_MODULE limbs
#define HEALTH_USES_LIMBS
#else
#if HEALTH_STYLE == HEALTH_WOUNDS
#define HEALTH_MODULE wounds
#define HEALTH_USES_LIMBS
#endif
#endif
#endif

#if WIELD_STYLE == WIELD_SINGLE
#define WIELD_MODULE single
#else
#if WIELD_STYLE == WIELD_MULTIPLE
#define WIELD_MODULE multiple
#else
#if WIELD_STYLE == WIELD_LIMBS
#define WIELD_MODULE limbs
#endif
#endif
#endif

#if ARMOR_STYLE == ARMOR_SIMPLE_SLOTS
#define ARMOR_MODULE simple_slots
#else
#if ARMOR_STYLE == ARMOR_COMPLEX_SLOTS
#define ARMOR_MODULE complex_slots
#else
#if ARMOR_STYLE == ARMOR_LIMBS
#define ARMOR_MODULE limbs
#endif
#endif
#endif

#if PULSE_STYLE == PULSE_NON_HEART_BEAT
#define PULSE_MODULE non_heart_beat
#else
#if PULSE_STYLE == PULSE_HEART_BEAT
#define PULSE_MODULE heart_beat
#endif
#endif

#if BLOW_STYLE == BLOW_SIMPLE
#define BLOW_MODULE simple
#else
#if BLOW_STYLE == BLOW_TYPES
#define BLOW_MODULE types
#endif
#endif

#if FORMULA_STYLE == FORMULA_SIMPLE
#define FORMULA_MODULE simple
#else
#if FORMULA_STYLE == FORMULA_STATS
#define FORMULA_MODULE stats
#else
#if FORMULA_STYLE == FORMULA_SKILLS
#define FORMULA_MODULE skills
#endif
#endif
#endif

#if ADVANCEMENT_STYLE == ADVANCEMENT_SIMPLE
#define ADVANCEMENT_MODULE simple
#endif

#endif
