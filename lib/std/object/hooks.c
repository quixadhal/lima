/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <hooks.h>
#include <driver/function.h>

//:MODULE
//
//The hook module is included as part of OBJECT, and allows a general method
//of allowing keeping track of and calling hooks, along with a method if
//specifying how multiple hooks should be resolved.

private mapping hooks = ([]);

//:FUNCTION add_hook
//add_hook(string tag, function hook) sets up the function 'hook' to be
//called whenever call_hooks(tag, ...) is done.  Note that if you want
//to remove the tag later, you have to do so with the _exact_ same function
//pointer.
//
//e.g.
//
//function my_hook = (: my_hook_func :);
//
// add_hook("foo", my_hook);
//
// remove_hook("foo", my_hook);
//
void add_hook(string tag, function hook) {
    array tmp = ({ hook });
    if (hooks[tag]) {
	// Make sure we only have one
	hooks[tag] -= tmp;
	hooks[tag] += tmp;
    } else
	hooks[tag] = tmp;
}

//:FUNCTION remove_hook
//
//Remove a hook added with add_hook.  The function pointer passed must be
//the same one that was passed to add_hook.
//
//see: add_hook
void remove_hook(string tag, function hook) {
    if (hooks[tag])
	hooks[tag] -= ({ hook });
}

//:FUNCTION hook_state
//
//hook_state(tag, hook, state) Either add or remove a hook based on the
//state 'state'
void hook_state(string tag, mixed hook, int state) {
    if (state)
	add_hook(tag, hook);
    else
	remove_hook(tag, hook);
}

//:FUNCTION call_hooks
//
//Call a set of hooks, with the specified method for resolving multiple
//hooks.  A setting from /include/hooks.h can be used, or a function pointer
//which is appropriate for implode()ing with the return values.
//
//E.g.
//
// call_hooks("foo", (: $1 + $2 :), 2) will return 2 + the sum of the return
//     values of the hooks
//
// but 2 + call_hooks("foo", HOOK_SUM) is faster.
//
//see: implode
//see: add_hook

varargs mixed call_hooks(string tag, mixed func, mixed start,
			 array args...) {
    array hooks_to_call;
    mixed tmp;
    function hook;

    if (hooks_to_call = hooks[tag]) {
	hooks_to_call = filter(hooks_to_call, 
			       (: !(functionp($1) & FP_OWNER_DESTED) :));
	hooks[tag] = hooks_to_call;
	
	if (!intp(func))
	    return implode(map(hooks_to_call, (: evaluate($1, $(args)...) :)),
			   func, start);
	switch (func) {
	case HOOK_IGNORE:
	    map(hooks_to_call, (: evaluate($1, $(args)...) :));
	    return 0;
	case HOOK_SUM:
	    foreach (hook in hooks_to_call)
		tmp += evaluate(hook, args...);
	    return tmp;
	case HOOK_LAND:
	    foreach (hook in hooks_to_call)
		if (!evaluate(hook, args...)) return 0;
	    return 1;
	case HOOK_LOR:
	    foreach (hook in hooks_to_call)
		if (tmp = evaluate(hook, args...))
		    return tmp;
	    return 0;
	case HOOK_YES_NO_ERROR:
	    foreach (hook in hooks_to_call) {
		tmp = evaluate(hook, args...);
		if (!tmp || stringp(tmp)) return tmp;
	    }
	    return (start || 1);
	default:
	    error("Unknown hook type in call_hooks.\n");
	}
    } else {
	if (!intp(func))
	    return start;

	switch (func) {
	case HOOK_IGNORE:
	case HOOK_SUM:
	case HOOK_LOR:
	    return 0;
	case HOOK_LAND:
	    return 1;
	case HOOK_YES_NO_ERROR:
	    return (start || 1);
	default:
	    error("Unknown hook type in call_hooks.\n");
	}
    }
}
