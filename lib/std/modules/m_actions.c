/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>
#include <commands.h>

void add_hook(string, function);
void remove_hook(string, function);
object query_target();

void action_arrival(object);
void action_departure(object);

private nosave function arrival_fn = (: action_arrival :);
private nosave function departure_fn = (: action_departure :);

private function            my_hook;
private nosave string array response_queue = ({ });
private int                 delay_time = 5;
private string array        my_actions;

private object              env;

mixed need_hooks() { return 1; }

object query_body()
{
  return this_object();
}

object query_shell_ob()
{
  return this_object();
}

string array query_actions()
{
  return my_actions;
}

//:FUNCTION set_delay_time
// void set_delay_time(int);
// Sets the amount of time between possible actions. The minimum value
// is 5, which is default and is fine for most situations. For a large
// number of action objects, increasing this number (and increasing the
// chance that an action can be executed at the same time) could be
// beneficial.
void set_delay_time(int x)
{
  delay_time = x >= 5 ? x : 5;
}

int query_delay_time()
{
  return delay_time;
}

//:FUNCTION do_game_command
//Emulates handling of emotes and player commands for NPCs that inherit this
//module.  E.g. do_game_command("wield sword").  do_game_command("smile hap*").
void do_game_command(string str)
{
  object save_tu;
  array winner;
  string verb, argument;

  save_tu = this_user();
  set_this_user(this_object());

  verb = str;
  sscanf(verb, "%s %s", verb, argument);

  winner = CMD_D->find_cmd_in_path(verb, ({ CMD_DIR_PLAYER "/" }));
  if(arrayp(winner))
    winner[0]->call_main("", "", "", "", "", "", argument);
  else if(environment())
  {
    mixed result = parse_sentence(str);

    if(stringp(result))
      write(result);
  }

  set_this_user(save_tu);
}

private void do_respond()
{
  mixed cmd = response_queue[0];
  if(stringp(cmd))
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
void respond(string str)
{
  response_queue += ({ str });
  call_out((: do_respond :), random(3));
  if(!my_hook)
  {
    my_hook = (: remove_call_out :);
    add_hook("remove", my_hook);
  }
}

// Return the number of listeners (player in room)
int query_listeners()
{
  object *inv;
  if (!env)
    return 0;
  if(inv = all_inventory(env))
    return sizeof(filter(inv, (: $1->query_link() :)));
  return 0;
}

// The periodic action call_out
void actions()
{
  if(!this_object()->query_target() && sizeof(my_actions))
    do_game_command(choice(my_actions));
  if(query_listeners())
    call_out("actions", delay_time);
}

// If last player leaves, remove call_out
private void action_departure(object who)
{
  if (who->query_link())
  {
    if(!query_listeners())
      remove_call_out("actions");
  }
}

// If first player arrives, add call_out
private void action_arrival(object who)
{
  if (who->query_link())
  {
    if(query_listeners() == 1)
      call_out("actions", delay_time);
  }
}

// It moves the "object_arrived/left" hooks (which are associated with a room)
// when the action monster moves
void action_movement()
{
  if(env)
  {
    env->remove_hook("object_arrived", arrival_fn);
    env->remove_hook("object_left", departure_fn);
  }

  env = environment();
  env->add_hook("object_arrived", arrival_fn );
  env->add_hook("object_left", departure_fn );

}

void start_actions()
{

  add_hook("move", (: action_movement :));
  if(env = environment(this_object()))
  { 
    env->add_hook("object_arrived", arrival_fn );
    env->add_hook("object_left", departure_fn );
  }

  if(query_listeners())
    call_out("actions", delay_time);
}


//:FUNCTION set_actions
// protected void set_actions(int chance, string array actions);
// This function sets a list of actions and a delay, 
// and chooses one to be executed time each time that delay elapses.
// This function should only be called from within setup().
protected void set_actions(int delay, string array actions)
{
  delay_time = delay;
  my_actions = actions;
  start_actions();
}
