/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** global.h
**
** These are standard definitions for using the Lima Mudlib.  You probably
** should not change anything in here unless you know what you're doing :-)
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <config.h>
#include <mudlib.h>

#define mud_name()		MUD_NAME
#define driver_version()	__VERSION__

#define wiz_dir(x)		sprintf(WIZ_DIR"/%s",(x)->query_userid())
#define un_article(x)		(stringp(x) ?\
			implode(explode(x," ")-({"a","an","the"})," ") : x)
//#define force_look()		(environment(this_body())->do_looking())

//#define DOC(f,s)	string __DOC_##f () { return s ; }
#define DOC(f,s)
//#define DOC_MODULE(s)	string __DOCM () { return s ; }
#define DOC_MODULE(s)
#define DOC_COMMAND(s)	string __DOCC () { return s ; }

#define TELL_BUG(p,x) (tell_user(p,sprintf("%s: %O\n",__FILE__,(x))))

#define BBUG(x) TELL_BUG("rust",(x))
#define DBBUG(x) TELL_BUG("deathblade",(x))
#define ZBUG(x) TELL_BUG("zifnab",(x))
#define ZABUG(x) TELL_BUG("zakk", (x))

#endif // __GLOBAL_H__
