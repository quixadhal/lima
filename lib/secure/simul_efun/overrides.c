/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <playerflags.h>
#include <commands.h>

object this_body();
int check_privilege(mixed);


nomask varargs void ed(string file, mixed func)
{
    error("ed() not available");
}

nomask int exec(object target, object src)
{
    error("exec() not available");
}

nomask object find_object( string str )
{
    object who;

    if ( !this_user() )
    {
	// Unless someone can come up with a
	// reason for a set_this_player() here
	return 0;
    }

    if ( !str )
	return 0;

    who = efun::find_object( str );

    if( who )
	if( interactive(who) )
	    if(who->query_hidden()>this_body()->query_level()) return 0;

    return who;
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


nomask object query_snoop(object ob){
  
  if(!check_privilege(1))
    return 0;
  return efun::query_snoop(ob);

}

nomask void shutdown()
{
    if ( check_privilege(1) )
	efun::shutdown();
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
	if(GROUP_D->adminp(targ))
	    tell_object(targ,"You are no longer being snooped.\n");
    }
    result = snoopee ? efun::snoop(this_user(), snoopee)
    : efun::snoop(this_user());
    if (!result) {
	write("Failed.\n");
    } else {
	write("Ok.\n");
	if(GROUP_D->adminp(snoopee))
	    tell_object(snoopee,sprintf("%s starts to snoop you!\n",
		this_body()->query_name()));
    }
    return result;
}



nomask object* users()
{
    int i;

    if ( !this_body() )
    {
	return filter(efun::users(), function (object ob) {
	    object body = ob->query_body();
	    if (!body) return 0;
	    return !body->query_hidden();
	});
    }

    if ( GROUP_D->adminp(this_user()) )
	return efun::users();

    return filter(efun::users(), function (object ob) {
	object body = ob->query_body();
	if (!body) return 0;
	return this_body()->query_level() >= body->query_hidden();
    });
}


nomask object * all_users()
{
    string * valid;
    string prev_file;

    valid = ({
	PLAYER,
	ANNOUNCE_D,
	FINGER_D,
	NCHANNEL_D,
    });
    prev_file = file_name(previous_object());
    if ( !prev_file )
	return 0;

    sscanf(prev_file, "%s#%*s", prev_file);
    if ( member_array(prev_file, valid) == -1 )
	return 0;

    return efun::users();
}
