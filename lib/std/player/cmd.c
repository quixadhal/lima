/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** cmd.c -- general command processing
*/

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

/* ### probably shouldn't be here (simul conversion) */
// currently doesn't have to be here since we're #included in player which
// inherits PARSER which inherits this



object query_link();			// in /std/player.c
string * query_path();			// in /std/player/path.c
int move(object location);		// in /std/object/move.c
mixed expand_if_alias(string input);	// in /std/player/alias.c
object query_mailer();			// in /std/player/mailbase.c


/* forward decl */
string history_and_alias_processing( string arg );


private static string * nonsense_msgs = ({
"A valiant attempt.\n",
"You can't be serious.\n",
"Even a candle is not that dim!\n",
"An interesting idea.\n",
"What a concept!\n",
"I think you've got better things to do.\n",
"Get serious.\n",
"Getting desperate?\n",
});

string nonsense()
{
    return nonsense_msgs[random(sizeof(nonsense_msgs))];
}



varargs nomask int do_game_command(string str, int debug)
{
    mixed result;
    string tmp;

    /*
    ** We can't try parsing the user has no environment.  We should
    ** move them to the VOID area and continue processing.
    */
    if ( !environment(this_object()) )
    {
	write("Oops!  You're lost.  Moving to the void...\n");
	move(load_object(VOID_ROOM));
	force_look();
    }

    /*
    ** Tmp hack, load the verb if not already loaded.
    */
    if(sscanf(str,"%s %s", result, tmp) > 0)
	load_object(CMD_DIR_VERBS+"/"+result);
    else
	load_object(CMD_DIR_VERBS+"/"+str);

    /*
    ** Parse the player's input
    */
    result = parse_sentence(str, debug);

    /*
    ** If a 1 was returned, then nothing more needs to be done.
    */
    if ( result == 1 )
	return 1;

    /*
    ** If a string was returned, then the parser figured something out.
    ** Write it out and we're done.
    */
    if ( stringp(result) )
    {
	write(result);
	return 1;
    }

    // If in debug mode, we're done
    if (debug) return 1;

    /*
    ** Check if they typed an exit
    */
    result = parse_sentence("go " + str);
    if (result == 1)
        return 1;

    /* 'You can't go ...' is a parser generated message for general
       failure.  The go command is careful to return explicit
       error messages if the command makes sense, so we can safely
       ignore it. */
    if (stringp(result) && result[0..12] != "You can't go ") {
        write(result);
        return 1;
    }
}

nomask void force_game_command(string str)
{
    object save_this_user = this_user();

    set_this_player(query_link());
    do_game_command(str);
    write(nonsense());
    set_this_player(save_this_user);
}

