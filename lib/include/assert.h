/* Do not remove the headers from this file! see /USAGE for more info. */

#ifdef DEBUG
#define ASSERT(x)	if (x) {} else { error(sprintf(\
		    "Assertation failed!  (%s) File: %s\n", x, __FILE__)); }
#else
#define ASSERT(x)
#endif
