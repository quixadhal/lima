/* Do not remove the headers from this file! see /USAGE for more info. */

/* Megaboz's attempt at a home cmd  4-23-94 */

#include <mudlib.h>
#include <playerflags.h>

inherit CMD;

mapping homes = ([]);

object find_home(string name) {
    if (homes[name]) return homes[name];

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
        if(file_name(environment(this_body())) == this_body()->query_home())
	{
            this_body()->simple_action( "$N $vtwitch briefly.\n");
	    return;
	}
	msgs = this_body()->get_player_message("home");
	if (environment(this_body()))
            this_body()->my_action( "You go home.\n");
            this_body()->other_action( msgs[1]);

	home = this_body()->query_home();
	if (home == "CLONE") {
	    ob = find_home(this_body()->query_name());
	    this_body()->move(ob);
	}
	else
	    this_body()->move(home);

	msgs = this_body()->get_player_message("menter");
	if (environment(this_body()))
	    tell_room(environment(this_body()), msgs[1], 0, ({ this_body() }));

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
        this_body()->simple_action( "$N $vtwitch briefly.\n" );
        return;
    }
    this_body()->simple_action("$N $vleave to visit " + capitalize(arg) +
      "'s home.\n");

    this_body()->move(home);

    tell_room(environment(this_body()), this_body()->query_name() +
      " dropped by to check on " + capitalize(arg) + ".\n",
      0, ({ this_body() }));

    if(!(this_body()->test_flag(F_BRIEF)))
	this_body()->do_game_command("look");

    return;
}

int help(){
    write("Usage: home [player]\nUsed with no arguments, will take you to your workroom.\n");
    write("You can also do:  home <wizard> to visit the home\nof another wizard.\n");
    return 1;
}
