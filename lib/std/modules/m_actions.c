/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

void add_hook(string, function);

private function	my_hook;
private string *	response_queue = ({});

object query_body() {
    return this_object();
}

object query_shell_ob() {
    return this_object();
}

//:FUNCTION do_game_command
//Emulates handling of emotes and player commands for NPCs that inherit this
//module.  E.g. do_game_command("wield sword").  do_game_command("smile hap*").
 void do_game_command(string str) {
    object save_tu;
    array winner;
    string verb, argument;

    save_tu = this_user();
    set_this_player(this_object());

    verb = str;
    sscanf(verb, "%s %s", verb, argument);
    
  
    winner = CMD_D->find_cmd_in_path(verb, ({ CMD_DIR_PLAYER "/" }));
    if (arrayp(winner)) {
        winner[0]->call_main("","","","","","",argument);
    }
    else if (environment()) {
	mixed result = parse_sentence(str);

	if (stringp(result))
	    write(result);
    }

    set_this_player(save_tu);
}

private void do_respond() {
    mixed cmd = response_queue[0];
    
    if (stringp(cmd))
	do_game_command(cmd);
    else
	evaluate(cmd);
    response_queue = response_queue[1..];
}

//:FUNCTION respond
//Does a command [using do_game_command] after a small delay.  This is the
//best way to have NPCs react to events, as a slight delay makes it a bit
//more believable, and also if you do things immediately, the action may
//PRECEDE the thing you are responding to.  Example: Troll which attacks
//anyone who says 'foo'.  I say 'foo', you and the troll are in the room.
//If the message is delivered to the troll before you, the troll will attack
//me BEFORE the message gets to you.
void respond(string str) {
    response_queue += ({ str });
    call_out( (: do_respond :), random(3));
    if (!my_hook) {
	my_hook = (: remove_call_out :);
	add_hook("remove", my_hook);
    }
}

//### Have set_actions() etc check if anyone is listening.

//:FUNCTION set_actions
//If you call set_actions(chance, actions), every 5 seconds, the monster has
//a '1 in chance' probability of doing one of the commands from the array
//of strings 'actions'.
protected void set_actions(int chance, string *actions) {
//### use scripts?
#if 0
cmd_mode = CMD_RANDOM;
    chance_or_delay = chance;
    my_actions = actions;
    start_up();
#endif
}
