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

void create()
{
  ::create();
  no_redirection();
}

private void main(string orig_input)
{
    
    string what = orig_input[(strsrch(orig_input," "))+1..];
    object targ_user = find_body(orig_input[0..strsrch(orig_input, " ")-1]);
    string s;

    tell_object(targ_user, this_body()->query_name()+" forced you to: "+what+"\n");
    tell_object(this_user(), "You forced "+targ_user->query_name()+" to: "+what+"\n");

    s = sprintf("%s forces %s to (%s) [%s]\n",
		this_user()->query_userid(), targ_user->query_link()->query_userid(),
		what, ctime(time()));
    LOG_D->log(LOG_FORCE, s);

    targ_user->query_link()->force_me(what);
}
