/* Do not remove the headers from this file! see /USAGE for more info. */

#ifndef HOOKS_H
#define HOOKS_H

#define HOOK_IGNORE		0 // ignore the return values
#define HOOK_SUM		1 // return the sum
#define HOOK_LAND		2 // return 1 iff all the hooks return 1
#define HOOK_LOR		3 // return 1 if any hook returns 1
#define HOOK_YES_NO_ERROR	4 // 1, 0, error like the parser

#endif
