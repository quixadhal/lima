/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** remote.c -- Remote MUD modules.
**
** To be used by objects which need to call functions on remote muds
**
** 13-Apr-95.  Beek.  Created.
*/

#include <mudlib.h>

DOC_MODULE("The remote module offers the call_remote() function for calling functions on other muds.");

DOC(call_remote, "allows you to execute a call_other on a remote mud.");

/* True varargs would be nice here */
void call_remote(string mud, string obj, string func, mixed *args) {
    REMOTE_D->remote_call(mud, obj, func, args);
}
    
DOC(call_broadcast, "allows you to execute a call_other on ALL other muds.");

void call_broadcast(string obj, string func, mixed *args) {
    REMOTE_D->broadcast_call(obj, func, args);
}

DOC(eval_remote, "will evaluate and return a result from a remote mud.");

void eval_remote(string mud, string obj, string func, mixed *args,
		 function result_func)
{
    REMOTE_D->remote_eval(mud, obj, func, args, result_func);
}

DOC(eval_broadcast, "will evaluate and return results from each remote mud.");

void eval_broadcast(string obj, string func, mixed *args,
		    function result_func)
{
    REMOTE_D->broadcast_eval(obj, func, args, result_func);
}


DOC(login_remote, "logs a user into to a remote mud");
void login_remote(string mud, string password)
{
    REMOTE_D->login_remote(mud, password);
}


DOC(is_logged_in, "returns whether a particular user is logged into the specified mud.");
int is_logged_in(object user, string mud)
{
    return REMOTE_D->is_logged_in(user, mud);
}

DOC(auth_call, "perform an authenticated/privileged call to a remote mud, carrying over the current (stack-based) privilege level.");
void auth_call(string mud, string obj, string func, mixed *args)
{
    REMOTE_D->auth_call(mud, obj, func, args);
}

DOC(auth_eval, "perform an authenticated/privileged evaluation (and result return) on a remote mud, carrying over the current (stack-based) privilege level.");
void auth_eval(string mud, string obj, string func, mixed *args,
	       function result_func)
{
    REMOTE_D->auth_eval(mud, obj, func, args, result_func);
}


DOC(muds_available, "returns information about the muds in the net, their connection status, and which this_user() has logged into.");
mixed * muds_available()
{
    return REMOTE_D->muds_available();
}

DOC(query_local_name, "returns the 'name' of the local mud");
string query_local_name()
{
    return REMOTE_D->query_local_name();
}
