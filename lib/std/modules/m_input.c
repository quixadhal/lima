/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** input.c -- modules to aid in working with the input system
**
** 950501, Deathblade: Created.
*/

private static object	input_user;

varargs nomask void modal_push(function input_func,
			       mixed prompt_func,
			       function secure,
			       function return_to_func
			       )
{
    if ( input_user && this_user() != input_user )
	error("user mismatch -- already assigned to a user\n");

    input_user = this_user();
    input_user->modal_push(input_func, prompt_func, secure, return_to_func);
}

nomask void modal_push_char(function callback)
{
    input_user->modal_push_char(callback);
}

nomask void modal_pop()
{
    input_user->modal_pop();
    input_user = 0;
}

varargs nomask void modal_func(function input_func,
			       mixed prompt_func,
			       int secure
			       )
{
    input_user->modal_func(input_func, prompt_func, secure);
}

varargs nomask void modal_simple(function input_func, int secure)
{
    this_user()->modal_simple(input_func, secure);
}

nomask void modal_pass(string str)
{
    input_user->modal_pass(str);
}

nomask int modal_stack_size()
{
    return input_user->modal_stack_size();
}
