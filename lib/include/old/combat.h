/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __COMBAT_H__
#define __COMBAT_H__

#define RES_DISARM		1 /* This blow should disarm it's target */
#define RES_MIGHT_BE_FATAL	2 /* A potentially fatal blow */
#define RES_NO_RESISTANCE	4 /* Target is asleep, or willing to die */
#define RES_NONE		8 /* Ignore this result */
#define RES_KNOCKDOWN		16 /* This blow should knockdown it's target */
#define RES_KNOCKOUT		32 /* This blow should knock out it's target */
#define RES_FATAL		64 /* This blow _is_ fatal, no matter what */
#define RES_NONPHYSICAL		128 /* Not a physical blow.  E.g. normal armor
				      shouldn't deflect it */

#endif /* __COMBAT_H__ */
