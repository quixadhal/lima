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
#include <move.h>
#include <hooks.h>
#include <size.h>

// Files we need to inherit --
inherit MONSTER;
//inherit M_GRAMMAR;
inherit M_ACCESS;
inherit M_SMARTMOVE;

inherit __DIR__ "player/quests";
inherit __DIR__ "player/mailbase";
inherit __DIR__ "player/newsdata";
inherit __DIR__ "player/path";
inherit __DIR__ "player/cmd";
inherit __DIR__ "player/help";
inherit __DIR__ "player/bodyshell";
inherit __DIR__ "player/wizfuncs";
inherit "/std/money";

#ifdef USE_SKILLS
inherit __DIR__  "player/skills";
#endif
#ifdef USE_TITLES
inherit __DIR__ "player/title";
#endif
#ifdef USE_SIMPLE_LEVEL
inherit __DIR__ "player/simple_level";
#endif

// Global variables --
private string name = "guest";
private string describe;
private string invis_name;
private string nickname;
private string start_location;
private string reply;
private string array channel_list = ({ });
private string plan;
private string array auto_load = ({ });
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
    if ( !is_visible() ) return invis_name;
    if ( test_flag(F_DEAD) ) return "A ghost";
    return cap_name;
}

nomask string query_userid()
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
    if(wizardp(this_user()))
       {
	 return WIZARD_START;
       }
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



void
load_autoload() 
{
    object ob;
    int l;

    if(!auto_load || (arrayp(auto_load)&&!sizeof(auto_load)))return;

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
	    if (ob->move(this_object()) != MOVE_OK)
		if (ob->move(start_location) != MOVE_OK)
		    if (ob->move(wizardp(this_user()) ? WIZARD_START : START) 
			!= MOVE_OK)
			ob->remove();
	}
    }
    auto_load = ({ });
}


void save_autoload() 
{
    object *inv;
    string *str;
    int    l;


    auto_load = ({ });
    inv = all_inventory(this_object());
    l = sizeof(inv);
    while (l--) {
	if (inv[l]->query_autoload()) {
	    auto_load += ({ base_name( inv[l] ) });
	}
    }
}

/* initialize various internal things */
private nomask void init_cmd_hook()
{
    object mailbox;
    int idx;

    link = previous_object();

    mailbox = MAIL_D->get_mailbox(query_userid());

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

#ifdef USE_MASS
    set_mass(100);
#endif
#ifdef USE_SIZE
    set_size(VERY_LARGE);
#endif
    set_max_capacity(VERY_LARGE);

    start_shell();
    load_autoload();
}

private nomask void finish_enter_game()
{
    NCHANNEL_D->deliver_emote("wiz_announce", 0,
			      sprintf("enters %s.", mud_name()));

    /* move the body.  make sure this comes before the simple_action */
    if ( move(start_location) != MOVE_OK &&
	 move(wizardp(this_user()) ? WIZARD_START : START) != MOVE_OK &&
	 move(VOID_ROOM) != MOVE_OK )
    {
	write("Uh-oh, you have no environment.\n");
	return;
    }

    /* we don't want other people to get the extra newlines */
    write("\n");
    if(is_visible())
      simple_action("$N $venter "+mud_name()+".\n");
    write("\n");

    NCHANNEL_D->register_channels(channel_list);

    do_game_command("look");
}

nomask void su_enter_game()
{
    init_cmd_hook();

    NCHANNEL_D->deliver_emote("wiz_announce", 0,
			      sprintf("enters %s.", mud_name()));

    if(is_visible())
      simple_action("$N $venter "+mud_name()+".\n");

    NCHANNEL_D->register_channels(channel_list);
}

void enter_game(int is_new)
{
    init_cmd_hook();

    if ( is_new && wizardp(link) )
    {
	write("\n"
	      "Hi, new wiz! Tuning you in to all the mud's important channels.\n"
	      "Doing: wiz /on\n"
	      "Doing: chan news /on   (you'll see when new news is posted.)\n"
	      "Doing: gossip /on\n"
	      "Doing: newbie /on\n"
	      "Doing: announce /on\n"
	      "\n");

	/* these will be registered later */
	channel_list = ({ "wiz_wiz", "plyr_news", "plyr_gossip",
			  "plyr_newbie", "wiz_announce" });

	/* So the hapless new wizard doesn't get spammed. */
	set_ilog_time(time());
    }
    else if ( is_new )
    {
	write("\n"
	      "Tuning in the newbie channel for you.  (newbie /on)\n"
	      "Tuning in the gossip channel for you.  (gossip /on)\n"
	      "\n");

	/* these will be registered later */
	channel_list = ({ "plyr_gossip", "plyr_newbie" });
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

	DID_D->dump_did_info(query_ilog_time(),
			     ({ "",
				"Changes since you last logged in",
				"********************************",
				"" }),
			     0,
			     (: finish_enter_game :));
	set_ilog_time(time());
    }
    else
    {
	finish_enter_game();
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

void remove(int leave_link)
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
    MAIL_D->unload_mailbox(query_userid());
    unload_mailer();
    LAST_LOGIN_D->register_last(query_userid());
    if ( link && !leave_link)
    {
	link->remove();
    }
    ::remove();
}

varargs void quit(int leave_link)
{
    if ( !clonep() )
    {
	::remove();
	return;
    }

    if (is_visible())
	simple_action("$N $vhave left "+mud_name()+".\n");

    save_autoload();

    NCHANNEL_D->deliver_emote("wiz_announce", 0,
			      sprintf("has left %s.", mud_name()));
    NCHANNEL_D->unregister_all();

    if (environment() && !wizardp(link))
	start_location = file_name(environment());

    remove(leave_link);
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
    if(is_visible())
      simple_action("$N $vhave gone link-dead.\n");
    NCHANNEL_D->deliver_emote("wiz_announce", 0,
			      sprintf("has gone link-dead.", mud_name()));
    call_out("remove",300);
    if(query_shell_ob()->get_variable("save_scrollback"))
      catching_scrollback = 1;
    
}

void reconnect()
{
    link = previous_object();
    remove_call_out("remove");
    if(is_visible())
      simple_action("$N $vhave reconnected.\n");

    NCHANNEL_D->deliver_emote("wiz_announce", 0,
			      sprintf("has reconnected.", mud_name()));

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
      if(is_visible())
	simple_action("If $n $vwere mortal, $n would now no longer be mortal.\n");
	heal_us(10000);
	stop_fight();
	return;
    }

    if(is_visible())
    simple_action("$N $vhave kicked the bucket, and $vare now pushing up the daisies.\n");
    tell_object(this_object(), "\n\n   ****  You have died  ****\n\n"
      "A pity, really.  Way too many people dying these days for me to just patch\n"
      "everyone up.  Oh well, you'll live.\n");
    rack_up_a_death();
    link->switch_body(GHOST, 1);

#ifdef DEATH_MESSAGES
    {
	// another option is to choose the message here based on player level
	// or something, instead of just letting action() pick at random.
	// something like: 
	//  action(({ this_object()}),
	//     (MESSAGES_D->get_messages("player_death"))[query_level()/5])[1];
	string msg = action(({this_object()}), 
			    MESSAGES_D->get_messages("player-death"))[1];
	(users() - ({ query_link() }))->tell(msg);
    }
#endif
}


void clean_up()
{
    quit();
}


int id(string arg)
{
    if(!is_visible() && arg == lower_case(invis_name))
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
    if (describe)
	return in_room_desc() + "\n" + describe +"\n";
    else
	return in_room_desc() + "\n" + cap_name + " is boring and hasn't described " + query_reflexive() + ".\n";
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

void tell(string message) {
    tell_object(link, message);
}

//### temp hack. be both user and body
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

/* verb interaction */
mixed indirect_give_obj_to_liv(object ob, object liv) {
    return 1;
}
