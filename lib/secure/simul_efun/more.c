/* Do not remove the headers from this file! see /USAGE for more info. */

#include <more.h>

object this_body();
varargs void tell(object, string, int);
mixed get_user_variable(string varname);

private nomask int default_num()
{
    int t = get_user_variable("MORE");
    int num = 20;

    if ( stringp(t) )
	t = to_int(t);
    if ( t )
	num = t;
    return num;
}

//:FUNCTION more
//more(arg) starts up more to display the array of lines 'arg'.  If arg is
//a string, it is exploded around "\n".  An optional second argument gives
//the number of lines per chunk.  An optional continuation function will
//be evaluated when the "more" is completed.
varargs nomask void more(mixed arg, int num, function continuation,
			 int output_flags)
{
    if (stringp(arg))
	arg = explode(arg, "\n");
    else
	if (!arrayp(arg))
	    return;

    if (!sizeof(arg))
	return;

    if (!num)
	num = default_num();

    if (sizeof(arg) < num) {
	foreach (string line in arg) {
	    tell(this_user(), line + "\n", output_flags);
	}
	if ( continuation )
	    evaluate(continuation);
	return;
    }

    new(MORE_OB, MORE_LINES, arg, num, continuation, output_flags);
}

//:FUNCTION more_file
//more_file(arg) starts up more to display the single file 'arg' if 'arg'
//is a string, or more than one file if 'arg' is an array of strings.
//An optional second argument gives the number of lines per chunk.  An
//optional continuation function will be evaluated when the "more" is
//completed.
varargs nomask void more_file(mixed arg, int num, function continuation,
			      int output_flags)
{
    if (stringp(arg)) 
	arg = ({ arg });
    else
	if (!arrayp(arg) || !sizeof(arg))
	    return;
    
    if (!num)
	num = default_num();

    new(MORE_OB, MORE_FILES, arg, num, continuation, output_flags);
}
