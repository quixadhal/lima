/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** setbit.h
**
** Bad file name for historical reasons, but these are the standard flags
** for objects in the system.
*/

#ifndef __SETBIT_H__
#define __SETBIT_H__

#include <flags.h>


#define FLAMING			MakeFlag(STD_FLAGS, 1)
#define TURN_ON_OFF		MakeFlag(STD_FLAGS, 3)
#define INVIS			MakeFlag(STD_FLAGS, 2)
#define TOUCHED			MakeFlag(STD_FLAGS, 4)
#define DESTROYABLE		MakeFlag(STD_FLAGS, 5)
#define KNOCKED_DOWN		MakeFlag(STD_FLAGS, 6)
#define TURN_OR_FLIPABLE	MakeFlag(STD_FLAGS, 7)
#define EDIBLE			MakeFlag(STD_FLAGS, 8)
#define DRINKABLE		MakeFlag(STD_FLAGS, 9)
#define BOARDABLE		MakeFlag(STD_FLAGS, 10)
#define IMPLEMENT		MakeFlag(STD_FLAGS, 11)
#define NO_AUTO_TAKE		MakeFlag(STD_FLAGS, 12)
#define NO_THIEF_FLAG		MakeFlag(STD_FLAGS, 13)
#define ATTACHED		MakeFlag(STD_FLAGS, 14)
#define CLIMBABLE		MakeFlag(STD_FLAGS, 15)
#define TRANSPARENT		MakeFlag(STD_FLAGS, 16)
#define READABLE		MakeFlag(STD_FLAGS, 17)
#define AIR_ROOM		MakeFlag(STD_FLAGS, 18)
#define MAZE			MakeFlag(STD_FLAGS, 19)
#define F_CAN_TIE_TO		MakeFlag(STD_FLAGS, 20)

#endif /* __SETBIT_H__ */
