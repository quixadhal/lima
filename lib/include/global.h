/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** global.h
**
** These are standard definitions for using the Lima Mudlib.  You probably
** should not change anything in here unless you know what you're doing :-)
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#ifndef __SENSIBLE_MODIFIERS__
#define nosave static
#define protected static
#endif

#include <config.h>
#include <mudlib.h>

#define mud_name()		MUD_NAME
#define driver_version()	__VERSION__

#define TELL_BUG(p,x) (tell_user(p,sprintf("%s: %O\n",__FILE__,(x))))

#define IBUG(x) TELL_BUG("iizuka",(x))
#define BBUG(x) TELL_BUG("rust",(x))
#define DBBUG(x) TELL_BUG("deathblade",(x))
#define ZBUG(x) TELL_BUG("zifnab",(x))
#define ZABUG(x) TELL_BUG("zakk", (x))
#define TBUG(x) TELL_BUG("tigran",(x))
#define RABUG(x) TELL_BUG("rassilon",(x))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(a)    (((a) < 0) ? -(a) : (a))
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#endif // __GLOBAL_H__
