/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_INPUT;


#define BLANKLINE "                                                                              "
#define PROMPT "*"


private nomask void handle_piping(string arg)
{
    if( arg == "**" || arg == ".")
    {
	write("Done.\n");
        modal_pop();
	return;
    }

    resend("/cmds/player/say", "say " + arg);
}

nomask private void main()
{
    write("Entering converse mode. Type '**' or '.' to quit.\n");
    write("-------------------------------------------------\n");
    modal_push((: handle_piping :),
#if 0 // Clients can't handle \r without \n well - Beek
	       "\r" BLANKLINE "\r" PROMPT "\r"
#else
	       PROMPT "\b"
#endif
	       );
}
