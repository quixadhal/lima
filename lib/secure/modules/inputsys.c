/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** inputsys.c
**
** Handles the input system processing for the player.
**
** 94-Aug-11.  Deathblade.  Created.
** 95-Apr-29.  Deathblade.  Converted to function ptrs.
** 95-May-20.  Rust.        Added char mode support.
** 95-Jul-20.  Rust. 	Added clear_input_stack for sw_user.c 's benefit.
** 95-Jul-20.  Beek.    Modified clear_input_stack to be more robust and
                        to allow objects to clean up
** 95-Jul-20.  Beek.    Prompts can be strings as well as functions
*/

#include <driver/function.h>

#define INPUT_NORMAL	0
#define INPUT_AUTO_POP	1
#define INPUT_CHAR_MODE	2

class input_info
{
    function	input_func;
    mixed	prompt;
    int		secure;
    function	return_to_func;
    int		input_type;
}
private static class input_info *	modal_stack = ({ });

private static int	dispatching_to;

private nomask void dispatch_modal_input(string str);


/*
** push_handler()
**
** Push a handler onto the modal stack.  The stack is grown as
** necessary to accomodate the new element.
*/
private nomask void push_handler(function input_func,
				 mixed prompt,
				 int secure,
				 function return_to_func,
				 int input_type
				 )
{
    class input_info info;

    info = new(class input_info);
    info->input_func		= input_func;
    info->prompt		= prompt;
    info->secure		= secure;
    info->return_to_func	= return_to_func;
    info->input_type		= input_type;

    modal_stack += ({ info });

    if ( info->input_type == INPUT_CHAR_MODE )
    {
	get_char((: dispatch_modal_input :), info->secure);
    }
    else	
    {
	input_to((: dispatch_modal_input :), info->secure);
    }
}

/*
** modal_push()
** modal_pop()
** modal_func()
**
** Handle the pushing, popping, and altering of the input handlers
** on the stack.
*/
varargs nomask void modal_push(function input_func,
			       mixed prompt,
			       int secure,
			       function return_to_func
			       )
{
    push_handler(input_func, prompt, secure, return_to_func,
		 INPUT_NORMAL);
}

nomask void modal_pop()
{
    /*
    ** Erase/pop the handler at the top level
    */
    modal_stack = modal_stack[0..<2];
}

varargs nomask void modal_func(function input_func,
			       mixed prompt,
			       int secure
			       )
{
    modal_stack[<1]->input_func = input_func;
    if ( prompt )
	modal_stack[<1]->prompt = prompt;
    modal_stack[<1]->secure = secure;
}

private nomask class input_info get_top_handler()
{
    while ( sizeof(modal_stack) )
    {
	class input_info info;

	/*
	** Get the top of the stack and make sure the func is valid
	*/
	info = modal_stack[<1];
	if ( !(functionp(info->input_func) & FP_OWNER_DESTED) )
	    return info;

	modal_stack = modal_stack[0..<2];
    }
    return 0;
}

nomask void modal_recapture()
{
    class input_info info;
    string prompt;

    if ( !(info = get_top_handler()) )
	return;

    /* auto-pop (modal_simple()) and char handlers don't have prompts */
    if ( info->input_type == INPUT_NORMAL && info->prompt )
    {
	prompt = evaluate(info->prompt);
	if ( prompt )
	    write(prompt);
    }
    if ( info->input_type == INPUT_CHAR_MODE )
    {
	get_char((: dispatch_modal_input :), info->secure);
    }
    else
    {
	input_to((: dispatch_modal_input :), info->secure);
    }
}

/*
** modal_simple()
**
** This function is used for very simple input handling (such as
** retrieving a single line of input).  It is much like modal_push()
** but the handler with automatically be popped after the first line
** of input is dispatched.
**
** This can be used as a direct replacement for input_to().
**
** NOTE: for multiple inputs, the standard push/pop is encouraged
** for efficiency reasons.
*/
varargs nomask void modal_simple(function input_func, int secure)
{
    push_handler(input_func, 0, secure, 0, INPUT_AUTO_POP);
}

/*
** modal_pass()
**
** Pass a string of input to the next input handler.  This is used
** by a handler when it cannot process input and would like it to
** return to the next handler down while still retaining control.
*/
nomask void modal_pass(string str)
{
    class input_info info;

    if ( !dispatching_to )
	error("no handlers to bubble to");

    info = modal_stack[--dispatching_to - 1];
    evaluate(info->input_func, str);	/* ### how to indicate passed? */
}

/*
** dispatch_modal_input()
**
** Dispatch the command as appropriate.
*/
private nomask void dispatch_modal_input(mixed str)
{
    class input_info info;

    /*
    ** Just in case the stack is empty (an error is thrown), pass the
    ** input to parse_line()
    **
    ** ### this prolly has to change
    */
    if ( !(info = get_top_handler()) )
    {
        this_body()->parse_line(str);
        return;
    }

    /* auto-pop _before_ dispatching, so we pop the correct handler */
    if ( info->input_type == INPUT_AUTO_POP )
        modal_pop();

    dispatching_to = sizeof(modal_stack);

    evaluate(info->input_func, str);

    /* reset to 0 in case we don't come thru here ("!" usage) */
    dispatching_to = 0;

    if ( this_object() )
	modal_recapture();
}

nomask void modal_push_char(function input_func)
{
    push_handler(input_func, 0, 1, 0, INPUT_CHAR_MODE);
}



/*
** process_input()
**
** In the absence of an input_to() (happens when an uncaught error occurs),
** or when the user uses the "!" syntax, then the driver will call this apply
** with the input.  We will dispatch this to the BOTTOM handler of the stack
** which will be an input shell.
**
** Note that we retain the use of input_to() so that we can use its
** "secure" setting.
*/
private nomask string process_input(string str)
{
    class input_info info;

    if(!sizeof(modal_stack))
      this_body()->start_shell();

    info = modal_stack[0];

    dispatching_to = 0;

    if( functionp(info->input_func) & FP_OWNER_DESTED )
      {
	modal_pop();
	process_input(str);
	return;
      }
    evaluate(info->input_func, str);

    if ( this_object() )
	modal_recapture();
}

/*
** force_me()
**
** Force a line of input to the user's bottom level input handler.
*/
nomask void force_me(string str)
{
    object save_this_user = this_user();

    set_this_player(this_object());
    process_input(str);
    set_this_player(save_this_user);
}


int stat_me()
{
    if ( check_privilege(1) )
	printf("INPUT STACK:\n%O\n", modal_stack);
}

nomask void clear_input_stack()
{
    class input_info top;

    while (sizeof(modal_stack))	{
	if (catch {
	    top = get_top_handler();
	    modal_pop();
	    top->input_func(-1); // a bit of a kludge, but zero is used
	}) {
	    write_file("/tmp/bad_handler",
		       sprintf("Error in input_func(-1):\n\tinput_func: %O\n\tprompt: %O\n", top->input_func, top->prompt));
	}
    }
}
