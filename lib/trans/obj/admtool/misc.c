/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** misc.c -- miscellaneous administration
**
** 961209, Deathblade: created
*/

#include <mudlib.h>

void std_handler(string str);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_MISC	"(AdmTool:misc) [hmq?] > "


private nomask void write_misc_menu()
{
    write("Administration Tool: Miscellaneous administration\n"
	  "\n"
	  "    h        - rebuild help data\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void receive_misc_input(string str)
{
    switch ( str )
    {
    case "h":
	HELP_D->rebuild_data();
	break;

    case "?":
	write_misc_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_misc_menu()
{
    modal_func((: receive_misc_input :), PROMPT_MISC);
    write_misc_menu();
}
