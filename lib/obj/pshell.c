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

static void execute_command(string * argv, string original_input)
{
    mixed tmp;
    object winner;
    string argument;
    
    /* BEGINNING OF EXPANSION */

    // In some shells, this is the hook for doing username completion,
    // globbing, flag pre-parsing, etc...  In others, it's used to execute
    // code encased in ` `'s.
    argv = map(argv, (: expand_one_argument :));
    if(!argv)
	return;
    argv -= ({ "" });

    // ### wtf is this?
    // Hmm, I might undo this one...  the only reason this is here is to 
    // allow \\$ to work right.  \$ can work right in other ways....
    argv = map(argv, (: stringp($1) ? replace_string($1, "\\$","$") : $1 :));

    // If there is a local shell command that matches our input, try to
    // execute it.
    evaluate(tmp=dispatch[argv[0]], argv);
    if(tmp)
	return;


    /* END OF EXPANSION */

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
	return;
    }

    if ( sizeof(argv) > 1 )
	argument = implode(argv[1..], " ");

    winner->call_main(argument);
}
