/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Force another user to perform a command
**
** 950930, Ranma@Koko Wa: added logging
** ???: created by somebody :-)
*/

#include <mudlib.h>
#include <log.h>

inherit CMD;

#define SYNTAX "Usage:  force <living> <command>\n"


private void main(mixed * arg)
{
    string what = implode(arg[1], " ");
    object targ_user = arg[0]->query_link();
    string s;

    tell_object(arg[0], this_body()->query_name()+" forced you to: "+what+"\n");
    tell_object(this_user(), "You forced "+arg[0]->query_name()+" to: "+what+"\n");

    s = sprintf("%s forces %s to (%s) [%s]\n",
		this_user()->query_userid(), targ_user->query_userid(),
		what, ctime(time()));
    LOG_D->log(LOG_FORCE, s);

    targ_user->force_me(what);
}
