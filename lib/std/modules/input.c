/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** input.c -- modules to aid in working with the input system
**
** 95-May-01. Deathblade. Created.
*/

varargs nomask void modal_push(function input_func,
			       mixed prompt_func,
			       function secure,
			       function return_to_func
			       )
{
    this_user()->modal_push(input_func, prompt_func, secure, return_to_func);
}

nomask void modal_push_char(function callback)
{
    this_user()->modal_push_char(callback);
}

nomask void modal_pop()
{
    this_user()->modal_pop();
}

varargs nomask void modal_func(function input_func,
			       mixed prompt_func,
			       int secure
			       )
{
    this_user()->modal_func(input_func, prompt_func, secure);
}

nomask void modal_recapture()
{
    this_user()->modal_recapture();
}

varargs nomask void modal_simple(function input_func, int secure)
{
    this_user()->modal_simple(input_func, secure);
}

nomask void modal_pass(string str)
{
    this_user()->modal_pass(str);
}
