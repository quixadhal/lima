/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** pshell.c -- player shell object
**
** Simple shell for the players.
**
** 950817, Deathblade: created.
*/

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

inherit SHELL;
inherit M_COMPLETE;


string query_shellname()
{
    return "Player Shell";
}

static int execute_command(string * argv, string original_input)
{
    object winner;
    string argument;

    winner = CMD_D->find_cmd_in_path(argv[0], ({ CMD_DIR_PLAYER "/" }));
    if ( !winner )
    {
	if (!this_body()->do_game_command(original_input))
	  {
//## This is a hack until the parser can tell me if a word is a verb.
	    if(is_file(CMD_DIR_VERBS "/" + argv[0] + ".c"))
	      write(this_body()->nonsense());
	    else
	      printf("I don't know the word: %s.\n", argv[0]);
	  }
	return 1;
    }

    if ( sizeof(argv) > 1 )
	argument = implode(argv[1..], " ");

    winner->call_main(argument);
    return 1;
}

private nomask string expand_one_argument(string arg)
{
    mixed expansion;

    if ( strlen(arg) <= 1 || arg[<1] != '*' )
	return arg;

    expansion = complete_user(arg[0..<2]);
    if ( stringp(expansion) )
	return expansion;

    return arg;
}

static string * expand_arguments(string * argv)
{
    return map(argv, (: expand_one_argument :));
}
