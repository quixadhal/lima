/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** flags.h
**
** Defines the macros and constants used for flag handling.  All flags in
** the system are identified with a single, unique flag indicator (usually
** represented with the 'which' variable).  These unique values are created
** with macros in this header.  The defined sets of flags are adminstrated
** in this file to prevent conflicts (although many of the actual flags in
** a set are defined in other headers).
**
** The functions to manipulate flags are defined in /std/base/flags.c:
**
**  int test_flag(int which)	test a flag -- returns 0 or 1
**  void set_flag(int which)	set a flag
**  void clear_flag(int which)	clear a flag
**
**  void assign_flag(int which, int state)	set or clear a flag
**  int get_flags(int set_key)			return a whole flag set
*/
#ifndef __FLAGS_H__
#define __FLAGS_H__


/*
** Flag keys are composed of two parts: an "administered" flag
** set identifier and the flag index.
**
** The set identifiers are administered here to prevent conflict
** among sets.  Many of the flag keys are defined in other
** headers.
**
** Keys are composed with the MakeFlag() macro.
*/
#define MakeFlag(s,i)	(((s) << 5) | ((i) & 0x1F))

#define FlagSet(w)	((w) >> 5)
#define FlagIndex(w)	((w) & 0x1F)


/*
** Administered sets
*/
#define STD_FLAGS	0	/* standard flags (setbit.h) */
#define PLAYER_FLAGS	2	/* player.c flags (playerflags.h) */
#define PLAYER_NP_FLAGS	3	/* player.c non-persist (playerflags.h) */
#define MAILBASE_FLAGS	5	/* mailbase.c flags */


/*
** MAILBASE_FLAGS
*/
#define F_BIFF		MakeFlag(MAILBASE_FLAGS, 0)


#endif /* __FLAGS_H__ */
