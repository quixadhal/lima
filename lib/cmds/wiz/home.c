/* Do not remove the headers from this file! see /USAGE for more info. */

/* Megaboz's attempt at a home cmd  4-23-94 */

//:COMMAND
//USAGE:  home
//        home <wizard name>
//
//This command will take you to your workroom assuming that there is
//a file called workroom.c in your home directory.
//If you do not have a workroom you will be placed in a "virtual"
//temporary workroom.
//You can also go to another wizards workroom by home <wizard's name>,

#include <playerflags.h>

inherit CMD;

mapping homes = ([]);

object find_home(string name)
{
  if (homes[name])
    return homes[name];

  homes[name] = new(TEMP_WORKROOM);
  homes[name]->set_owner(name);
  return homes[name];
}

private void main(string arg)
{
  object ob;
  string *msgs;
  string home;

  if (!arg)
  {
    object env = environment(this_body());
    if( env )
    {
      if( file_name(env) == this_body()->query_home())
      {
        this_body()->simple_action("$N $vtwitch briefly.");
        return;
      }
      msgs = this_body()->get_player_message("home");
      this_body()->my_action( "You go home.");
      this_body()->other_action( msgs[1]);

    }
    home = this_body()->query_home();
    if (home == "CLONE")
    {
      ob = find_home(this_body()->query_name());
      this_body()->move(ob);
    }
    else
      this_body()->move(home);

    msgs = this_body()->get_player_message("menter");
    tell_environment(this_body(), msgs[1], 0, ({ this_body() }));

    if ( !(this_body()->test_flag(F_BRIEF)) )
      this_body()->do_game_command("look");

    return;
  }

  home = WIZ_DIR + "/" + arg + "/workroom";
  if ( file_size(home + ".c") <= 0 )
  {
    out(capitalize(arg) + " does not have a home.\n");
    return;
  }

  if( file_name( environment( this_body())) == home )
  {
    this_body()->simple_action("$N $vtwitch briefly.");
    return;
  }
  this_body()->simple_action("$N $vleave to visit " + capitalize(arg) +
      "'s home.");

  this_body()->move(home);

  tell_environment(this_body(), this_body()->query_name() +
      " dropped by to check on " + capitalize(arg) + ".\n",
      0, ({ this_body() }));

  if(!(this_body()->test_flag(F_BRIEF)))
    this_body()->do_game_command("look");

  return;
}