/* Do not remove the headers from this file! see /USAGE for more info. */

#ifdef DEBUG
#define assert(x)	if (x) {} else { error(sprintf(\
		    "Assertation failed: \"##x\" (File: %s)\n",  __FILE__)); }
#else
#define assert(x)
#endif
#define ASSERT(x)	if (x) {} else { error(sprintf(\
		    "Assertation failed: \"##x\" (File: %s)\n",  __FILE__)); }
