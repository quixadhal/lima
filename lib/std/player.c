/* Do not remove the headers from this file! see /USAGE for more info. */

// /std/player.c  Written after login.c 1-12-94
// Rust@ZorkMUD
// with mods by alot of us:
// including:   Nevyn@ZorkMUD (GUE)
//              Peregrin @ZorkMUD (GUE)
// Beek ... Deathblade ... The rest of the Zorkmud staff ...
//
#include <daemons.h>
#include <mudlib.h>
#include <config.h>
#include <security.h>
#include <setbit.h>
#include <playerflags.h>
#include <commands.h>


// Files we need to inherit --
inherit MONSTER;
//inherit M_GRAMMAR;
inherit M_ACCESS;

inherit "/std/player/quests";
inherit "/std/player/mailbase";
inherit "/std/player/newsdata";
inherit "/std/player/path";
inherit "/std/player/cmd";
inherit "/std/player/help";
inherit "/std/player/bodyshell";
inherit "/std/player/wizfuncs";
inherit "/std/money";

#ifdef USE_SKILLS
inherit "/std/player/skills";
#endif
#ifdef USE_TITLES
inherit "/std/player/title";
#endif


// Global variables --
private string name = "guest";
private string describe;
private string invis_name;
private string nickname;
private string start_location;
private string reply;
private int more_chunk;
private string * channel_list = ({ });
private string plan;

// part of patch for handling auto loading  -- Pere@ZorkMUD
private string * auto_load = ({ });

private static object link;
private static string cap_name;
private static int catching_scrollback;

#ifdef USE_STATS
inherit M_BODY_STATS;

int to_hit_base() {
    return 50 - query_agi();
}

void refresh_stats() {
    bodystats::refresh_stats();
}
#endif

// interfaces for other objects to manipulate our global variables

nomask object query_link(){  return link; }

string short() { return query_name(); }

string a_short() { return query_name(); }

string the_short() { return query_name(); }

string query_name()
{
    if ( invis_name == cap_name || !invis_name ) invis_name = "Someone";
    if ( test_flag(INVIS) ) return invis_name;
    if ( test_flag(F_DEAD) ) return "A ghost";
    return cap_name;
}

nomask string query_real_name()
{
    return name;
}

int set_start_location(string str)
{
    if ( !stringp(str) )
	return 0;

    if ( load_object(str) )
    {
	start_location = str;
        return 1;
    }

    return 0;
}

string query_start_location()
{
    if ( start_location )
	return start_location;
    return START;
}


#ifdef EVERYONE_HAS_A_PLAN

nomask string query_plan()
{
    return plan;
}

nomask void set_plan(string new_plan)
{
    if ( this_body() != this_object() )
	error("illegal attempt to set plan\n");

    plan = new_plan;
    save_me();
}

#endif /* EVERYONE_HAS_A_PLAN */


// load info for auto_loading   -- Peregrin@ZorkMUD

void
load_autoload() {
    object ob;
    int l;

    if(!auto_load || (pointerp(auto_load)&&!sizeof(auto_load)))return;
    //    write("Autoloading");
    if (l = sizeof(auto_load)) {
	while (l--) {
	    if (!stringp(auto_load[l]))  {
		write("[bad filename]  ");
		continue;
	    }
	    if (!stringp(read_file(auto_load[l]+".c",1,5))) {

		write(" [cannot find an item] ");
		printf("%s\n",auto_load[l]);
		continue;
	    }
	    ob = clone_object(auto_load[l]);
	    ob->init_arg();  // Rust.  This should pass an arg?
	    write("....");
	    if (ob->move(this_object()))
		if (ob->move(start_location))
		    if (ob->move(START))
			ob->remove();
	}
    }
    auto_load = ({ });
    //    write(" complete.\n");
}

// store info for auto_loading   -- Peregrin@ZorkMUD

void save_autoload() {
    object *inv;
    string *str;
    int    l;

    //    tell_object(this_object(),"Saving autoload");
    auto_load = ({ });
    inv = all_inventory(this_object());
    l = sizeof(inv);
    while (l--) {
	if (inv[l]->query_autoload()) {
	    //	    tell_object(this_object(),"....");
	    auto_load += ({ base_name( inv[l] ) });
	}
    }
    //    tell_object(this_object()," complete.\n");
}


void init_cmd_hook()
{
    object mailbox;
    int idx;

    link = previous_object();

    mailbox = MAIL_D->get_mailbox(query_real_name());

    idx = mailbox->first_unread_message();
    if ( idx == -1 )
    {
	mailbox->set_message_index(mailbox->query_message_count() - 1);
    }
    else
    {
        mailbox->set_message_index(idx);
	write("\n>>You have new mail<<\n\n");
    }

    add_id_no_plural( name );
    if (nickname)
	add_id_no_plural(nickname);

    NCHANNEL_D->register_channels(channel_list);
    NCHANNEL_D->deliver_string("wiz_announce", 
			    sprintf("[announce] %s enters %s.\n",
				    capitalize(name), mud_name()));

    set_mass(100);
    set_max_capacity(100);

    start_shell();
    load_autoload();

#undef WORLD_IS_FLAT            
#ifdef WORLD_IS_FLAT
    call_out((: parse_sentence(name) :), 0);
#endif
}


void enter_game(int is_new)
{
    init_cmd_hook();

    if ( move(start_location) && move(START) )
    {
	write("Uh-oh, you have no environment.\n");
	return;
    }

    simple_action("$N $venter "+mud_name()+".\n");

    if ( is_new && wizardp(link) )
    {
	write("\nHi, new wiz! Tuning you in to all the mud's important channels.\n");
	write("Doing: wiz /on\n");
//	link->force_me("wchan wiz /on");
	write("Doing: chan news /on   (you'll see when new news is posted.)\n");
// for some reason aliases don't catch from a force_me from here, so use the
// full cmd. (probably a better idea anyway P-)
//	link->force_me("chan news /on");
	write("Doing: gossip /on\n");
//	link->force_me("chan gossip /on");
	write("Doing: newbie /on\n");
//	link->force_me("chan newbie /on");
        write("Doing: announce /on\n\n");
//        link->force_me("wchan announce /on");

	channel_list = ({ "wiz_wiz", "plyr_news", "plyr_gossip",
			  "plyr_newbie", "wiz_announce" });
	NCHANNEL_D->register_channels(channel_list);

	/* So the hapless new wizard doesn't get spammed. */
	set_ilog_time(time());
    }
    else if ( is_new )
    {
	write("\nTuning in the newbie channel for you.  (newbie /on)\n");
//	link->force_me("chan newbie /on");
	write("Tuning in the gossip channel for you.  (gossip /on)\n\n");
//	link->force_me("chan gossip /on");

	channel_list = ({ "plyr_gossip", "plyr_newbie" });
	NCHANNEL_D->register_channels(channel_list);
    }

    if ( wizardp(link) )
    {
	string login_file;

	// do .login stuff
	login_file = wiz_dir(this_object()) + "/.login";
	if( file_size(login_file) > 0 )
	{
	    map_array(explode(read_file(login_file),"\n"),
		      (: this_user()->force_me($1) :));
	}
    }

    do_game_command("look");

    /*
    ** For now, this must be after all other printed stuff cuz it will
    ** start a MORE_OB, which doesn't block the above printing.  At
    ** some point, we'll add continuation functions to MORE_OB and then
    ** this can be anywhere in the login sequence, with a continuation
    ** for the rest of the sequence.
    */
    if ( wizardp(link) )
    {
	DID_D->dump_did_info(query_ilog_time(),
			     ({ "Changes since you last logged in",
				"********************************",
				"" }));
	set_ilog_time(time());
    }
}

void save_me()
{
    /* prepare to save the shell data */
    bodyshell::prepare_to_save();

    if (previous_object()==this_object())
	save_autoload();

    unguarded( 1, (: save_object , USER_PATH(name) :) );
}

void remove()
{
    object ob;

    if ( !clonep() )
    {
	::remove();
	return;
    }

    save_me();

    if ( ob = query_shell_ob() )
	ob->remove();
    MAIL_D->unload_mailbox(query_real_name());
    unload_mailer();
    LAST_LOGIN_D->register_last(query_real_name());
    if ( link )
    {
	link->remove();
    }
    ::remove();
}

void quit()
{
    if ( !clonep() )
    {
	::remove();
	return;
    }

    if (!test_flag(INVIS))
	simple_action("$N $vhave left "+mud_name()+".\n");
    save_autoload();
    NCHANNEL_D->deliver_string("wiz_announce", 
			    sprintf("[announce] %s has left %s.\n",
				    capitalize(name), mud_name()));
    NCHANNEL_D->unregister_all();
    if (environment() && !wizardp(link))
	start_location = file_name(environment());
    remove();
}


string query_idle_string()
{
    int idle_time;
    string result="";
    if(interactive(link))
       idle_time = query_idle(link)/60;
    if(!idle_time)
      return "";

    // Don't worry about weeks or months :)
    if (idle_time > 24 * 60)
        result += " [idle " + number_of(idle_time/(24*60), "day") + "]";
    else if (idle_time > 60) 
        result += " [idle " + number_of(idle_time/60, "hour") + "]";
    else
        result += " [idle " + number_of(idle_time, "minute") + "]";

    return result;
}


// This is used by in_room_desc and by who, one of which truncates,
// one of which doesnt.  Both want an idle time.

string base_in_room_desc()
{
    string result;

    /* no description if invisible */
    if ( test_flag(INVIS) ) return 0;

    /* ensure the player has a title. set it if none (yet) */
#ifdef USE_TITLES
    if ( !title )
	title = sprintf("%s the title-less",cap_name);
    result = title;
#else
    result = cap_name;
#endif

    /* if they are link-dead, then prepend something... */
    if ( !link || !interactive(link) )
	result = "The lifeless body of " + result;
    return result;
}

string in_room_desc()
{
  return base_in_room_desc() + query_idle_string();
}

void set_reply(string o){
    reply = o;
}

string query_reply(){ return reply; }

void net_dead()
{
    simple_action("$N $vhave gone link-dead.\n");
    NCHANNEL_D->deliver_string("wiz_announce", 
			    sprintf("[announce] %s has gone link-dead.\n",
				    capitalize(name)));
    call_out("remove",300);
    if(query_shell_ob()->get_variable("save_scrollback"))
      catching_scrollback = 1;
    
}

void reconnect()
{
    link = previous_object();
    remove_call_out("remove");
    simple_action("$N $vhave reconnected.\n");

    NCHANNEL_D->deliver_string("wiz_announce",
			    sprintf("[announce] %s has reconnected.\n",
				    capitalize(name)));
    catching_scrollback = 0;
    if ( query_shell_ob() )
      query_shell_ob()->end_scrollback();

    start_shell();
}

nomask void reset_input_handler()
{
    if ( previous_object() != query_link() )
	error("illegal attempt to modify input processing\n");

    start_shell();
}

void die()
{
    if ( wizardp(link) )
    {
	simple_action("If $n $vwere mortal, $n would now no longer be mortal.\n");
	heal_us(10000);
	stop_fight();
	return;
    }

    simple_action("$N $vhave kicked the bucket, and $vare now pushing up the daisies.\n");
    tell_object(this_object(), "\n\n   ****  You have died  ****\n\n"
      "A pity, really.  Way too many people dying these days for me to just patch\n"
      "everyone up.  Oh well, you'll live.\n");
    rack_up_a_death();
    link->switch_body(GHOST, 1);
}


void clean_up()
{
    quit();
}


int id(string arg)
{
    if(test_flag(INVIS) && arg == lower_case(invis_name))
	return 1;

    return ::id(arg);
}

int stat_me()
{
    write(short()+"\n");
    write("Name: "+cap_name+"\n");
    ::stat_me();
    if ( link )
	link->stat_me();
    return 1;
}

void set_description(string str) {
  if(base_name(previous_object()) == CMD_OB_DESCRIBE)
    describe = str;
  save_me();
}

string our_description() {
    return in_room_desc() + "\n" + describe +"\n";
}

void set_nickname(string arg) {
    if (file_name(previous_object()) != CMD_OB_NICKNAME)
	error("Illegal call to set_nickname\n");

    remove_id( nickname );
    nickname = arg;
    add_id_no_plural(nickname);
}

string query_nickname() {
    return nickname;
}

private void create(string userid)
{
    if ( !clonep() )
	return;

    if ( base_name(previous_object()) != USER_OB )
	error("security violation: illegal attempt to change name\n");

    messages = ([]);

    monster::create();
#ifdef USE_STATS
    bodystats::create();
#endif

    /*
    ** Both sets of flags can only be set by this_object() -- this
    ** restriction is imposed by using the secure_closure feature
    ** of the flag handling code.  The second set (PLAYER_NP_FLAGS)
    ** is non-persistent (not saved).
    */
    /* Beek - note: this first check uses this_body() and hence is
              completely bogus. */
    configure_set(PLAYER_FLAGS, 0,
		  (: this_object() != this_body() :) );
    configure_set(PLAYER_NP_FLAGS, 1,
		  (: this_object() != this_body() ||
		   ($2 && $1 == 97 && previous_object() != find_object("/bin/player/_inactive")) :) 
        	   );

    set_long( (: our_description :) );

    cap_name = capitalize(userid);
    name = userid;
    unguarded(1, (: restore_object, USER_PATH(userid), 1 :));

    if ( !describe )
    {
	describe = (cap_name +
		    " is a boring player and hasn't described " +
		    query_reflexive() + ".\n"
		    );
    }

    // up to the player
    set_attack_speed(0);
}


/*
** Listen to channel messages and manipulate the channels
*/
void channel_rcv_string(string channel_name, string message)
{
    tell_object(this_object(), message);
}
void channel_rcv_soul(string channel_name, mixed * data)
{
    string msg;

    if ( data[0][0] == this_object() )
	msg = data[1][0];
    else if ( sizeof(data[0]) == 2 && data[0][1] != this_object() )
	msg = data[1][2];
    else
	msg = data[1][1];
	
    tell_object(this_object(), msg);
}
void channel_add(string which_channel)
{
    channel_list += ({ which_channel });
    NCHANNEL_D->register_channels(({ which_channel }));
}
void channel_remove(string which_channel)
{
    channel_list -= ({ which_channel });
    NCHANNEL_D->unregister_channels(({ which_channel }));
}
string * query_channel_list()
{
    return channel_list;
}

/*
** We need to catch stuff told to the body and relay it to the user ob
*/
private nomask void catch_tell(string message)
{
    if ( link )
        tell_object(link, message);
    if ( catching_scrollback && query_shell_ob())
      query_shell_ob()->add_scrollback(message);
}

/* ### temp hack. be both user and body */
nomask object query_body()
{
    return this_object();
}
nomask mixed * query_failures()
{
    return link->query_failures();
}
nomask void clear_failures()
{
    return link->clear_failures();
}
