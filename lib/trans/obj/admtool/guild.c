/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** guild.c -- Guild administration
**
** 961209, Deathblade: created
*/

#include <mudlib.h>

void std_handler(string str);
static nomask void do_one_arg(string arg_prompt, function fp, string arg);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_GUILD	"(AdmTool:guild) [mq?] > "


private nomask void write_guild_menu()
{
    write("Administration Tool: Guild administration\n"
	  "\n"
	  "    <none so far>\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void receive_guild_input(string str)
{
    switch ( str )
    {
    case "?":
	write_guild_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_guild_menu()
{
    modal_func((: receive_guild_input :), PROMPT_GUILD);
    write_guild_menu();
}
