/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;


private void main( mixed *arg, mapping f)
{
    object	target;
    mixed	output;

    target = arg[0];
    if( f["i"] )
	output = implode(({sprintf("Function list for %s",file_name(target))})+
sort_array(
	  map_array( functions( target ), (: sprintf("%-34s: %-34s",
		(function_exists($1, $(target)) ?
		  function_exists($1, $(target)) :
                "*STATIC*"), $1 ) :)), 1), "\n");
    else
	output =
	sprintf("Function list for %s:\n%-79#s\n",
	  file_name(target), implode(sort_array(functions(target), 1), "\n"));

	    out(output);
	}




