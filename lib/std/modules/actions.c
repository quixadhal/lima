/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

#define CMD_RANDOM		1
#define CMD_SEQUENTIAL		2
#define CMD_LOOP		3

private int		cmd_mode	= CMD_RANDOM;
private int		chance_or_delay;
private string * 	my_actions;
private int		per_flag;
private string *	response_queue = ({});

// Script stuff
class script{
  int id;
  mixed* actions;
  int*   delays;
}

private mapping scripts = ([]);
private int next_script_id = 1;
private int active_script;
private int current_step;
private int repeat;



object query_body() {
    return this_object();
}

static int valid_respond(string response){
  return active_script ? 0 : 1;
}

object query_shell_ob() {
    return this_object();
}

static void do_game_command(string str) {
    object winner, save_tp;
    string verb, argument;

    save_tp = this_player();
    set_this_player(this_object());

    verb = str;
    sscanf(verb, "%s %s", verb, argument);
    
    winner = CMD_D->find_cmd_in_path(verb, ({ CMD_DIR_PLAYER "/" }));
    if (winner)
	winner->call_main(argument);
    else if (environment()) {
	mixed result = parse_sentence(str);

	if (stringp(result))
	    write(result);
    }

    set_this_player(save_tp);
}

void do_respond() {
    do_game_command(response_queue[0]);
    response_queue = response_queue[1..];
}

void respond(string str) {
    if(!valid_respond(str))
      {
	return;
      }
    response_queue += ({ str });
    call_out( (: do_respond :), random(3));
}

private void periodic() {
    switch (cmd_mode) {
    case CMD_RANDOM:
	if (!random(chance_or_delay))
	    do_game_command(my_actions[random(sizeof(my_actions))]);
	break;
    }
    call_out( (: periodic :), 5);
}

private void start_up() {
    if (per_flag) return;
    call_out( (: periodic :), 5);
    per_flag = 1;
}

// don't want people doing this to users ...
static void set_actions(int chance, string *actions) {
    cmd_mode = CMD_RANDOM;
    chance_or_delay = chance;
    my_actions = actions;
    start_up();
}


// This will be rewritten as soon as call_outs are.
// By Rust.


void cancel_all_scripts()
{
    mixed *calls = my_call_outs();
    mixed call;

    /*
    ** Remove each call_out associated with a script.  Don't torch them all,
    ** because whatever inherited this module might have other call_outs.
    */
    foreach(call in calls)
    {
	remove_call_out(call[1]);
    }
}

void cancel_script(string scriptname)
{
  int id;
  mixed *calls = my_call_outs();
  mixed call;
  id = ((class script)scripts[scriptname])->id;
  foreach(call in calls)
    {
      if(arrayp(call[3]) && call[3][0] == id)
	{
	  remove_call_out(call[1]);
	  break;
	}
    }
}

int add_script(string scriptname, mixed script)
{
  class script s = new(class script);
  int* delays = ({});
  mixed* actions = ({});
  int i;
  s->id = next_script_id++;
  for(i=0; i < sizeof(script); i+=2)
    {
      delays += ({script[i]});
    }
  s->delays = delays;
  for(i=1; i < sizeof(script); i+=2)
    {
      actions += ({script[i]});
    }
  s->actions = actions;
  scripts[scriptname] = s;
  return s->id;
}
	


varargs int run_script(string scriptname, int rep)
{
  class script s= (class script)scripts[scriptname];
  int i;
  string* funcs;
  if(!s)
    return 0;
  if(active_script)
    return 0;

  repeat = rep;
  active_script = s->id;
  current_step = 0;
  funcs = allocate(sizeof(s->actions)+1);
  for(i=0;i<sizeof(s->actions);i++)
    funcs[i] = "handle_script";
  funcs += ({"finished"});
  call_out_chain(funcs,s->delays[0],s->id, s);

}

int handle_script(int scriptid, class script s)
{
  if(stringp(s->actions[current_step]))
    do_game_command(s->actions[current_step]);
  else evaluate(s->actions[current_step]);
  if(current_step+1 == sizeof(s->delays))
    return 0;
  return s->delays[++current_step];
}

void finished(int id, class script s)
{
  string* funcs;
  int i;


  active_script = 0;
  if(repeat--) 
    {
      current_step = 0;
      funcs = allocate(sizeof(s->actions)+1);
      for(i=0;i<sizeof(s->actions);i++)
	funcs[i] = "handle_script";
      funcs += ({"finished"});
      call_out_chain(funcs,s->delays[0],s->id, s);
    }
}

void remove()
{
    remove_call_out();
}
