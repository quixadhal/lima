/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>

// John
// Sep 7 94

private nosave mixed	get_response = 1;
private nosave mixed	drop_response = 1;
private nosave function	my_drop_hook, my_get_hook;

void add_hook(string, function);

//### hmm, are these necessary?  I think hook evaluation uses an evaluate
//### anyway...
private mixed prevent_drop() {
    return evaluate(drop_response);
}

private mixed prevent_get() {
    return evaluate(get_response);
}

//:FUNCTION set_getmsg
//set_getmsg(s) sets the error message that one gets when one tries to take
//an object.
void
set_getmsg( string s )
{
    get_response = s;
    if (!my_get_hook) {
	my_get_hook = (: prevent_get :);
	add_hook("prevent_get", my_get_hook);
    }
}

//:FUNCTION query_getmsg
//query_getmsg() returns the error message that one gets when one tries to take
//an object.
string
query_getmsg()
{
    if (stringp(get_response)) return get_response;
}

//:FUNCTION set_gettable
//set_gettable(1) makes an object gettable, while set_gettable(0) makes
//it not gettable.  If a function or string is passed, this has the
//same effect as calling set_getmsg().
void
set_gettable( mixed g )
{
    if (g == -1 || !g)
	get_response = 0;
    else
	get_response = 1;

    if(functionp(g) || stringp(g))
	get_response = g;

    if (!my_get_hook) {
	my_get_hook = (: prevent_get :);
	add_hook("prevent_get", my_get_hook);
    }
}

//:FUNCTION get
//Do some checks before getting an object.  Returns 1 if successful,
//otherwise 0 or a string error message. 
mixed get() { 
    object env;
    int tmp;
    
    if (get_response != MOVE_OK)
	return get_response;
    
    env = environment();
    while (env) {
	if (tmp = environment()->inventory_accessible());
	    return tmp;
	env = environment(env);
    }
    return MOVE_OK;
}

//:FUNCTION set_dropmsg
//Set the error message that one gets when one tries to drop an object
void
set_dropmsg( string s )
{
    drop_response = s;
    if (!my_drop_hook) {
	my_drop_hook = (: prevent_drop :);
	add_hook("prevent_drop", my_drop_hook);
    }
}

//:FUNCTION query_dropmsg
//returns the error message one gets when one tries to drop an object.
string
query_dropmsg()
{
    if (stringp(drop_response)) return drop_response;
}

//:FUNCTION set_droppable
//set_droppable(1) makes an object droppable, while set_droppable(0) makes
//it not droppable.  If a function or string is passed, this has the
//same effect as calling set_dropmsg().
void
set_droppable( int g )
{
    if (g == -1 || g == 0)
	drop_response = 0;
    else
	drop_response = 1;

    if (functionp(g) || stringp(g))
	drop_response = g;
    
    if (!my_drop_hook) {
	my_drop_hook = (: prevent_drop :);
	add_hook("prevent_drop", my_drop_hook);
    }
}

//:FUNCTION drop
//Do some checks before dropping an object.  Returns 1 if successful,
//otherwise 0 or a string error message. 
mixed
drop()
{
    if (drop_response != 1)
	return drop_response;

    return 1;
}

//:FUNCTION is_gettable
//return one if an object can be taken.
int is_gettable()
{
	return get_response == 1;
}


mapping lpscript_attributes() {
    return ([
        "getmsg" : ({ LPSCRIPT_STRING, "setup", "set_getmsg" }),
    ]);
}
