/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;


private void main( mixed *arg, mapping f)
{
    object	target;
    mixed	output;
    string	flag;

    target = arg[0];
    if( f["i"] )
	output = ({sprintf("Function list for %s",file_name(target))})+
	map_array( functions( target ), (: sprintf("%-34s: %-34s", $1,
					   (function_exists($1, $(target)) ?
					    function_exists($1, $(target)) :
					    "*STATIC*")) :));
    else
	output =
	sprintf("Function list for %s:\n%-79#s\n",
	  file_name(target), implode(functions(target),"\n"));

    clone_object(MORE_OB)->more_string(output);
}
