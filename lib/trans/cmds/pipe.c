/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_INPUT;


#define BLANKLINE " \
                                                                                  "
#define PROMPT "*"


private nomask void handle_piping(string verb, string arg)
{
    if( arg == "**" || arg == "." )
    {
	write("Done.\n");
        modal_pop();
	return;
    }

    this_user()->force_me(sprintf("%s %s", verb, arg));
}

nomask private void main(string arg)
{
    if ( !arg || arg == "" )
	write("Note:  No verb supplied, each line will be treated as its own command.\n");


    write("Entering pipe mode. Type '**' to quit.\n");
    modal_push((: handle_piping, arg :),
#if 0 // \r is evil.  See converse -Beek
	       (: "\r" BLANKLINE "\r" PROMPT "\r" :)
#else
	       "*\b"
#endif
	       );
}
