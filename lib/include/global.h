/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <config.h>

#define wiz_dir(x)		sprintf(WIZ_DIR"/%s",(x)->query_real_name())
#define fail(x)			(: notify_fail, (x) :)
#define un_article(x)		(stringp(x) ?\
			implode(explode(x," ")-({"a","an","the"})," ") : x)
#define force_look()		(environment(this_body())->do_looking())

//#define DOC(f,s)	string __DOC_##f () { return s ; }
#define DOC(f,s)
//#define DOC_MODULE(s)	string __DOCM () { return s ; }
#define DOC_MODULE(s)
#define DOC_COMMAND(s)	string __DOCC () { return s ; }

#define TELL_BUG(p,x) (find_user(p)?tell_object(find_user(p),sprintf("%s: %O\n",__FILE__,(x))):0)

#define BBUG(x) TELL_BUG("rust",(x))
#define DBBUG(x) TELL_BUG("deathblade",(x))
#define ZBUG(x) TELL_BUG("zifnab",(x))

#endif // __GLOBAL_H__
