/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <security.h>
inherit M_ACCESS;
create() { set_privilege(1); }


#include "/wiz/deathblade/eval.h"
mixed exec_foo(){ return __NO_ENVIRONMENT__;}
