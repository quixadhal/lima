/* Do not remove the headers from this file! see /USAGE for more info. */

#include <playerflags.h>
#include <commands.h>

object this_body();
int check_privilege(mixed priv);
int adminp(mixed m);
varargs void tell(object, string);


//:FUNCTION ed
//The ed() efun is not used by the LIMA mudlib, as we use the new ed
//functionality.  See ed_session.c
nomask varargs void ed(string file, mixed func)
{
    error("ed() not available\n");
}

//:FUNCTION exec
//The exec efun is never used by the LIMA mudlib, since only bodies are
//reconnected.
nomask int exec(object target, object src)
{
    error("exec() not available\n");
}

//:FUNCTION debug_info
//The debug_info efun is temporarily denied, until the driver is altered
//to use the valid_bind() apply.
nomask string debug_info(int operation, object ob)
{
    if ( !check_privilege(1) )
	error("debug_info() not available right now\n");
    return efun::debug_info(operation, ob);
}

//:FUNCTION input_to
//input_to should not be used; the stackable input system should be used
//instead.  See the modal_push() and modal_func() routines.
nomask varargs int input_to()
{
    error("input_to() should not be used.  Use modal_push()/modal_func()\n");
}

//:FUNCTION find_player
//find_body() and find_user() are used to find the body and connection objects,
//respectively.
nomask object find_player(string str)
{
    error("find_player() is obsolete.  Use find_body() instead.\n");
}

//:FUNCTION this_player
//this_body() and this_user() are used to find the body and connection objects,
//respectively.
nomask object this_player(string str)
{
    error("this_player() is obsolete.  Use this_body() or this_user() instead.\n");
}

//:FUNCTION destruct
//The destruct simul_efun guarantees that an object will always have remove()
//called in it if it is destructed by another object.  destruct() with no
//arguments destructs this_object WITHOUT calling remove, which is the
//correct way to destruct an object from within a remove() function.
//destruct() can also be passed an optional second arg, which is passed
//to the remove() routine.  The update command passes 1 as the second
//argument to indicate that a new copy will be reloaded immediately.
nomask varargs void destruct(object ob, mixed arg) {
    if (!nullp(ob)) { // were we called with an arg ?
	if (ob)
	    catch(ob->remove(arg));
    } else {
	// self destruct; presumably from non_object or some similar base
	ob = previous_object();
    }
    if (ob)
	efun::destruct(ob);
}

//:FUNCTION shutdown
//The shutdown simul exists to prevent anything less than priv 1 from shutting
//down the mud.
nomask void shutdown()
{
    if ( check_privilege(1) )
	efun::shutdown();
    else
	error("Insufficient privilege to shut down.\n");
}

//:FUNCTION query_snoop
//The query_snoop simul makes sure only priv 1 can check snoops
nomask object query_snoop(object ob)
{
    if(!check_privilege(1))
	return 0;
    return efun::query_snoop(ob);
}

//:FUNCTION query_snooping
//The query_snooping simul prevents non priv 1 objects from checking on snoops
nomask object query_snooping(object ob)
{
    if(!check_privilege(1))
	return 0;
    return efun::query_snooping(ob);
}

//:FUNCTION snoop
//The snoop simul does some security checks before allowing snooping
varargs nomask mixed snoop(mixed snoopee)
{
    object result;

    if (snoopee) {
	object body = snoopee->query_body();

	if (body && !body->test_flag(F_SNOOPABLE) && !check_privilege(1)) {
	    write("Failed (permission denied).\n");
	    return 0;
	}
	if (efun::query_snoop(snoopee)) {
	    write("Busy.\n");
	    return 0;
	}
	result = efun::snoop(this_user(), snoopee);
	if(result && adminp(snoopee))
	    tell(snoopee,sprintf("%s starts to snoop you!\n",
		this_body()->query_name()));
    } else {
	if (!efun::query_snooping(this_user())) {
	    write("Not snooping.\n");
	    return 0;
	}

	foreach (object u in users()) {
	    if (efun::query_snoop(u) == this_user() && adminp(u))
		tell(u, "You are no longer being snooped.\n");
	}
	result = efun::snoop(this_user());
    }
    if (!result) {
	write("Failed.\n");
    } else {
	write("Ok.\n");
    }
    return result;
}

varargs void tell_object()
{
    error("Use tell() instead.\n");
}

varargs void tell_room()
{
    error("Use tell_environment() or tell_from_inside() instead.\n");
}

varargs void shout()
{
    error("Use tell(users() - ({ this_user() }), msg) instead.\n");
}


//:FUNCTION say
//The say() efun is not used by the LIMA mudlib, in favor of the extensive
//messaging system in the body.  Consider using this_body()->other_action()
//instead of say().
void say(string m)
{
    error("say() not available. Consider using this_body()->other_action()\n");
}

void write(string str) {
    if (this_user())
	tell(this_user(), str);
    else
	debug_message("]" + str);
}

void printf(string format, array rest...) {
    if (this_user())
	tell(this_user(), sprintf(format, rest...));
    else
	debug_message("]" + sprintf(format, rest...));
}


/*
varargs int memory_info(object ob)
{
    error("memory_info() is disabled for now, pending a bugfix.\n");
}
*/
