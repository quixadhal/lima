/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** input.c -- modules to aid in working with the input system
**
** All functions in the interface are "static" so that only the object
** inheriting this module can call them.
**
**    modal_push()
**    modal_push_char()
**    modal_pop()
**    modal_func()
**    modal_simple()
**    modal_pass()
**    modal_stack_size()
**
**    input_one_arg()
**    input_two_args()
**    input_three_args()
**
** 961209, Deathblade: moved the input_*_args() functions here from admtool.
** 950501, Deathblade: Created.
*/

private static object	input_user;

static varargs nomask void modal_push(
    function input_func,
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

static nomask void modal_push_char(function callback)
{
    input_user->modal_push_char(callback);
}

static nomask void modal_pop()
{
    input_user->modal_pop();
    input_user = 0;
}

static varargs nomask void modal_func(
    function input_func,
    mixed prompt_func,
    int secure
    )
{
    input_user->modal_func(input_func, prompt_func, secure);
}

static varargs nomask void modal_simple(
    function input_func,
    mixed prompt,
    int secure
    )
{
    this_user()->modal_simple(input_func, prompt, secure);
}

static nomask void modal_pass(string str)
{
    input_user->modal_pass(str);
}

static nomask int modal_stack_size()
{
    return input_user->modal_stack_size();
}


//:FUNCTION input_one_arg
//Get a single argument and call a given function pointer with it.  If the
//argument isn't present (arg == 0), then the prompt is used to ask for
//the value.
static varargs nomask void input_one_arg(
    string arg_prompt,
    function fp,
    string arg
    )
{
    if ( !arg )
    {
	modal_simple(fp, arg_prompt);
	return;
    }

    evaluate(fp, arg);
}

private nomask void rcv_first_of_two(string arg2_prompt,
				     function fp,
				     string arg1)
{
    string arg2;

    if ( arg1 == "" )
    {
	write("Aborted.\n");
	return;
    }

    if ( sscanf(arg1, "%s %s", arg1, arg2) == 2 )
    {
	evaluate(fp, arg1, arg2);
    }
    else
    {
	modal_simple((: evaluate, fp, arg1 :), sprintf(arg2_prompt, arg1));
    }
}

//:FUNCTION input_two_args
//Get two one-word arguments and call a given function pointer with them.
//Some or all of the arguments may be passed in initially; if they are not
//all present, then the prompts are used to ask for the values.
static varargs nomask void input_two_args(
    string arg1_prompt,
    string arg2_prompt,
    function fp,
    string arg
    )
{
    if ( arg )
    {
	string arg2;

	if ( sscanf(arg, "%s %s", arg, arg2) == 2 )
	{
	    evaluate(fp, arg, arg2);
	}
	else
	{
	    rcv_first_of_two(arg2_prompt, fp, arg);
	}
    }
    else
    {
	modal_simple((: rcv_first_of_two, arg2_prompt, fp :), arg1_prompt);
    }
}

private nomask void rcv_last_of_three(string arg3_prompt,
				      function fp,
				      string arg1,
				      string arg2)
{   
    string arg3;
  
    if ( arg1 == "" )
    {
	write("Aborted.\n");
	return;
    }

    if ( sscanf(arg2, "%s %s", arg2, arg3) == 2 )
    {
	evaluate(fp, arg1, arg2, arg3);
    }
    else
    {
	modal_simple((: evaluate, fp, arg1, arg2 :),
		     sprintf(arg3_prompt, arg2));
    }
}

private nomask void rcv_second_of_three(string arg2_prompt,
					string arg3_prompt,
					function fp,
					string arg)
{
    string arg1, arg2, arg3;

    if ( sscanf(arg, "%s %s %s", arg1, arg2, arg3) == 3 )
    {
	evaluate(fp, arg1, arg2, arg3);
    }
    else if ( sscanf(arg, "%s %s", arg1, arg2) == 2 )
    {
	rcv_last_of_three(arg3_prompt, fp, arg1, arg2);
    }
    else
    {
	modal_simple((: rcv_last_of_three, arg3_prompt, fp, arg :),
		     sprintf(arg2_prompt, arg1));
    }
}

//:FUNCTION input_three_args
//Get three one-word arguments and call a given function pointer with them.
//Some or all of the arguments may be passed in initially; if they are not
//all present, then the prompts are used to ask for the values.
static varargs nomask void input_three_args(
    string arg1_prompt,
    string arg2_prompt,
    string arg3_prompt,
    function fp,
    string arg
    )
{
    string arg1, arg2, arg3;

    if ( arg )
    {
	if ( sscanf(arg, "%s %s %s", arg1, arg2, arg3) == 3 )
	{
	    evaluate(fp, arg1, arg2, arg3);
	}
	else if ( sscanf(arg, "%s %s", arg1, arg2) == 2 )
	{
	    rcv_last_of_three(arg3_prompt, fp, arg1, arg2);
	}
	else
	{
	    rcv_second_of_three(arg2_prompt, arg3_prompt, fp, arg);
	}
    }
    else
    {
	/* 0 Args */
	modal_simple((: rcv_second_of_three, arg2_prompt, arg3_prompt, fp :),
		     arg1_prompt);
    }
}
