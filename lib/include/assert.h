/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef __ASSERT_H__
#define __ASSERT_H__

/*
** assert() -- when debugging, assert a condition to be true
** ensure() -- ensure that a condition is true (debug or not)
*/

#ifdef DEBUG
# define assert(x)	if (x) {} else { error(sprintf(\
		    "Assertation failed: \"##x\" (File: %s)\n",  __FILE__)); }
#else
# define assert(x)
#endif

#define ENSURE(x)	if (x) {} else { error(sprintf(\
		    "Assertation failed: \"##x\" (File: %s)\n",  __FILE__)); }

//### backwards compat
#define ASSERT(x) ENSURE(x)

#endif /* __ASSERT_H__ */
