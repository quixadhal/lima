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
inherit M_ACCESS;
inherit M_SMARTMOVE;

#ifndef EVERYTHING_SAVES
private inherit M_SAVE; // don't want people calling load_from_string()
                        // externally
#endif

inherit __DIR__ "body/quests";
inherit __DIR__ "body/mailbase";
inherit __DIR__ "body/newsdata";
inherit __DIR__ "body/cmd";
inherit __DIR__ "body/help";
inherit __DIR__ "body/wizfuncs";
inherit __DIR__ "body/money";
inherit __DIR__ "body/start";
inherit __DIR__ "body/time";

#ifdef USE_SKILLS
inherit __DIR__ "body/skills";
#endif
#ifdef USE_TITLES
inherit __DIR__ "body/title";
#endif
#ifdef USE_SIMPLE_LEVEL
inherit __DIR__ "body/simple_level";
#endif
#ifdef USE_WIZ_POSITION
inherit __DIR__ "body/wiz_position";
#endif
#ifdef USE_GUILDS
inherit __DIR__ "body/guilds";
#endif

// Global variables --
private string name = "guest";
private string describe;
private string invis_name;
private string nickname;
private string reply;
private string array channel_list = ({ });
private string plan;
private static object link;
private static string cap_name;
private static int catching_scrollback;
private mixed saved_items;

#ifdef USE_STATS
inherit M_BODY_STATS;

int to_hit_base() {
    return 50 - query_agi();
}

void refresh_stats() {
    m_bodystats::refresh_stats();
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

string query_long_name()
{
    if (query_ghost())
	return "The ghost of " + cap_name;
#ifdef USE_TITLES
    return query_title();
#else
    return cap_name;
#endif
}

nomask string query_userid()
{
    return name;
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

/* initialize various internal things */
private nomask void init_cmd_hook()
{
    object mailbox;
    int idx;

    link = previous_object();

    mailbox = MAILBOX_D->get_mailbox(query_userid());

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

    if (saved_items) {
	load_from_string(saved_items, 1);
	saved_items = 0;
    }
}

private nomask void finish_enter_game()
{

    NCHANNEL_D->deliver_emote("announce", query_name(),
			      sprintf("enters %s.", mud_name()));

    /* move the body.  make sure this comes before the simple_action */
    if ( !move_to_start() )
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

nomask void su_enter_game(object where)
{
    init_cmd_hook();

//### this should go away once we torch the corresponding leave msg for 'su'
    NCHANNEL_D->deliver_emote("announce", query_name(),
			      sprintf("enters %s.", mud_name()));

    if ( is_visible() )
	simple_action("$N $venter "+mud_name()+".\n");

    NCHANNEL_D->register_channels(channel_list);

    move(where);
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
	channel_list = ({ "wiz", "news", "gossip",
			  "newbie", "announce" });

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
	channel_list = ({ "gossip", "newbie" });
    }

    if ( wizardp(link) )
    {
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
    object shell_ob = link && link->query_shell_ob();

    /* save the shell information */
    if ( shell_ob )
	shell_ob->save_me();

//### This check is bogus.  What should it be?
// This check also doesn't work for su's -- John
//    if (previous_object()==this_object())
	saved_items = save_to_string(1); // 1 meaning it is recursive.

    unguarded( 1, (: save_object , USER_PATH(name) :) );
    saved_items = 0;
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

    MAILBOX_D->unload_mailbox(query_userid());
    unload_mailer();
    LAST_LOGIN_D->register_last(query_userid());

    ::remove();
}

void quit()
{
    if ( !clonep() )
    {
	::remove();
	return;
    }

    if (is_visible())
	simple_action("$N $vhave left "+mud_name()+".\n");

    NCHANNEL_D->deliver_emote("announce", query_name(),
			      sprintf("has left %s.", mud_name()));
    NCHANNEL_D->unregister_channels();

#ifdef PLAYERS_START_WHERE_THEY_QUIT
    if (environment() && !wizardp(link))
	set_start_location(file_name(environment()));
#endif

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

    result = query_long_name();

    /* if they are link-dead, then prepend something... */
    if ( !link || !interactive(link) )
	result = "The lifeless body of " + result;

    return result;
}

string in_room_desc()
{
  return base_in_room_desc() + query_idle_string();
}

string query_formatted_desc(int num_chars)
{
    string idle_string;
    int i;
  
    idle_string = query_idle_string();
    if ( i = strlen(idle_string) )
    {
	num_chars -= (i + 1);
	idle_string = " " + idle_string;
    }
    return truncate(base_in_room_desc(), num_chars) +  idle_string;
}

void set_reply(string o){
    reply = o;
}

string query_reply(){ return reply; }

void net_dead()
{
//### add security here?

    if(is_visible())
	simple_action("$N $vhave gone link-dead.\n");

    NCHANNEL_D->deliver_emote("announce", query_name(),
			      sprintf("has gone link-dead.", mud_name()));

    if ( link && link->query_shell_ob()->get_variable("save_scrollback") )
	catching_scrollback = 1;
}

void reconnect(object new_link)
{
//### add security here?

    link = new_link;
    if(is_visible())
	simple_action("$N $vhave reconnected.\n");

    NCHANNEL_D->deliver_emote("announce", query_name(),
			      sprintf("has reconnected.", mud_name()));

    catching_scrollback = 0;
    if ( link->query_shell_ob() )
	link->query_shell_ob()->end_scrollback();
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

    set_hp(0);
    if(is_visible())
	simple_action("$N $vhave kicked the bucket, and $vare now pushing up the daisies.\n");
    receive_private_msg("\n\n   ****  You have died  ****\n\n"
			"A pity, really.  Way too many people dying these days for me to just patch\n"
			"everyone up.  Oh well, you'll live.\n",0,0);
    rack_up_a_death();

#ifdef DEATH_MESSAGES
    {
	// another option is to choose the message here based on player level
	// or something, instead of just letting action() pick at random.
	// something like: 
	//  action(({ this_object()}),
	//     (MESSAGES_D->get_messages("player_death"))[query_level()/5])[1];
	string msg = action(({this_object()}), 
			    MESSAGES_D->get_messages("player-death"))[1];
			    shout(msg);
    }
#endif
}


void clean_up()
{
    if ( link )
	link->quit();
    else
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

    if ( nickname )
	remove_id(nickname);

    nickname = arg;
    add_id_no_plural(nickname);
}

string query_nickname() {
    return nickname;
}

private void create(string userid)
{
    int idx;

    if ( !clonep() )
	return;

    if ( base_name(previous_object()) != USER_OB )
	error("security violation: illegal attempt to change name\n");

    messages = ([]);

    monster::create();
#ifdef USE_STATS
    m_bodystats::create();
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

#ifdef USE_GUILDS
    fix_guild_data();
#endif

    // up to the player
    set_attack_speed(0);

//### transition stuff for upgrading old channel data
    if ( (idx = member_array("plyr_news", channel_list)) != -1 )
	channel_list[idx] = "news";
    if ( (idx = member_array("plyr_gossip", channel_list)) != -1 )
	channel_list[idx] = "gossip";
    if ( (idx = member_array("plyr_newbie", channel_list)) != -1 )
	channel_list[idx] = "newbie";
    if ( (idx = member_array("wiz_wiz", channel_list)) != -1 )
	channel_list[idx] = "wiz";
    if ( (idx = member_array("wiz_errors", channel_list)) != -1 )
	channel_list[idx] = "errors";
    if ( (idx = member_array("wiz_announce", channel_list)) != -1 )
	channel_list[idx] = "announce";
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

//### temp hack. be both user and body
nomask object query_body()
{
    return this_object();
}
nomask object query_shell_ob()
{
    return link && link->query_shell_ob();
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

/* helper function to forward a message to the user object */
private nomask void forward_to_user(string msg)
{
    if ( link )
	link->receive_message(0, msg);
}

// Inside messages propogate upward and downward...
void receive_inside_msg(string msg, object array exclude, int message_type, 
			mixed other)
{
    forward_to_user(msg);

    ::receive_inside_msg(msg, exclude, message_type, other);
}

// Outside messages propogate downward
void receive_outside_msg(string msg, object array exclude, int message_type,
			 mixed other)
{
    forward_to_user(msg);

    ::receive_outside_msg(msg, exclude, message_type, other);
}

//Remote messages propogate just like an inside message by default
void receive_remote_msg(string msg, object array exclude, int message_type,
			mixed other)
{
    receive_inside_msg(msg, exclude, message_type, other);
}

// Private messages just go to the user object
void receive_private_msg(string msg, int message_type, mixed other)
{
    forward_to_user(msg);
}

int go_somewhere(string arg)
{
  object env;
  int    ret;

  if(!(ret=::go_somewhere(arg)) && (env = environment(this_object())))
    return env->go_somewhere(arg);

  return ret;
}

string inventory_header()
{
  return query_name() + " is carrying:\n";
}

int ob_state() 
{
  return -1;
}
