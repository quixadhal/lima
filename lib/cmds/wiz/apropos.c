/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Apropos command that interfaces with the DOC_D.
** John Viega (rust@virginia.edu)
**
** 15-APR-95 Created.
*/

#include <daemons.h>
#include <mudlib.h>

inherit CMD;

  /* for iwrap() */
inherit M_GLOB;

//:COMMAND
//Returns information on which mudlib functions contain the
//keyword passed, including a short description.

//### This currently doesn't work with the new DOC_D
    
//These are global to make life easier on me.

string output;
mapping apropos_info;

void add_info_to_output_2(string, mapping, string);

private
void
add_info_to_output(string func)
{
    mixed m = apropos_info[func];
    mixed k = keys(m);
    if(sizeof(k) == 1)
    {
	output += iwrap(sprintf("%15s (%s):  %s\n", func , k[0], m[k[0]]));
	return;
    }
    map_array(sort_array(k, 1), (: add_info_to_output_2 :), m, func);
}

private
void
add_info_to_output_2(string obj, mapping m, string func)
{
    output += iwrap(sprintf("%15s (%s):  %s\n", func, obj, m[obj]));
}

	      
private void
main(string s)
{
    write( "Apropos is Bugged. Bug Someone to fix it.\n")
;
    return;
    output = "";
    if(!s)
    {
      write("Usage: apropos <pattern>");
      return;
    }

    apropos_info = DOC_D->apropos_function(translate(s,1));
    map_array(sort_array(keys(apropos_info),1), (: add_info_to_output :));
    more(output);
}
