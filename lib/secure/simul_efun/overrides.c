/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <playerflags.h>
#include <commands.h>

object this_body();
int check_privilege(mixed priv);
int adminp(mixed m);


nomask varargs void ed(string file, mixed func)
{
    error("ed() not available");
}

nomask int exec(object target, object src)
{
    error("exec() not available");
}

nomask object find_player(string str)
{
    error("find_player() is obsolete.  Use find_body() instead.");
}

nomask int notify_fail(string msg)
{
    if ( this_body() )
	this_body()->set_notify_fail(msg);
    return 0;
}


nomask void shutdown()
{
    if ( check_privilege(1) )
	efun::shutdown();
}


nomask object query_snoop(object ob)
{
    if(!check_privilege(1))
	return 0;
    return efun::query_snoop(ob);
}

nomask object query_snooping(object ob)
{
    if(!check_privilege(1))
	return 0;
    return efun::query_snooping(ob);
}


varargs nomask mixed snoop(mixed snoopee)
{
    object result;

    if (snoopee && !snoopee->test_flag(F_SNOOPABLE) 
      && !check_privilege(1)) {
	write("Failed.\n");
	return 0;
    }
    if (snoopee && efun::query_snoop(snoopee)) {
	write("Busy.\n");
	return 0;
    }
    if(!snoopee){
	object targ;
	object* u;
	int i;
	u = users();
	i = sizeof(u); while(i--)
	    if(efun::query_snoop(u[i]) == this_user())
		targ = u[i];
	if(adminp(targ))
	    tell_object(targ,"You are no longer being snooped.\n");
    }
    result = snoopee ? efun::snoop(this_user(), snoopee)
    : efun::snoop(this_user());
    if (!result) {
	write("Failed.\n");
    } else {
	write("Ok.\n");
	if(adminp(snoopee))
	    tell_object(snoopee,sprintf("%s starts to snoop you!\n",
		this_body()->query_name()));
    }
    return result;
}
