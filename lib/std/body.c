/* Do not remove the headers from this file! see /USAGE for more info. */

//### lots of functions in here need to be autodoc'd

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
#include <combat.h>
#include <clean_up.h>

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
inherit __DIR__ "body/naming";

#ifdef USE_STATUS_LINE
inherit __DIR__ "body/status_line";
#endif
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
#ifdef USE_STATS
inherit M_BODY_STATS;
#endif

// Global variables --
private string reply;
private string array channel_list = ({ });
private string plan;
private static object link;
private mixed saved_items;

// interfaces for other objects to manipulate our global variables

//:FUNCTION query_link
//Return our link object
nomask object query_link()
{
    return link;
}


#ifdef EVERYONE_HAS_A_PLAN

//:FUNCTION query_plan
//Returns our plan
nomask string query_plan()
{
    return plan;
}

//:FUNCTION set_plan
//Sets our plan
nomask void set_plan(string new_plan)
{
    if ( this_body() != this_object() )
	error("illegal attempt to set plan\n");

    plan = new_plan;
    save_me();
}

#endif /* EVERYONE_HAS_A_PLAN */

static void update_for_new_body(mapping tmp) {
    /* nothing for now; can be overloaded for races that need it */
}

/* initialize various internal things */
//### needs a new name
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
	write("\n>>You have new mail<<\n");
    }

write( "\n" );
    naming_init_ids();

#ifdef USE_MASS
    set_mass(100);
#endif
#ifdef USE_SIZE
    set_size(VERY_LARGE);
#endif
    set_max_capacity(VERY_LARGE);

    if (saved_items) {
	string e;
	
	if (e = catch(load_from_string(saved_items, 1))) {
	    mapping tmp = restore_variable(saved_items);
	    
	    if (tmp["#base_name#"] != base_name(this_object())) {
		update_for_new_body(tmp);
		tmp["#base_name#"] = base_name(this_object());
		load_from_string(save_variable(tmp), 1);
	    } else
		error("Rethrown: " + e);
	}
	saved_items = 0;
    }
}

nomask void su_enter_game(object where)
{
    init_cmd_hook();

    //### this should go away once we torch the corresponding leave msg for 'su'
    CHANNEL_D->deliver_emote("announce", query_name(),
      sprintf("enters %s.", mud_name()));

    if ( is_visible() )
	simple_action("$N $venter "+mud_name()+".");

    CHANNEL_D->register_channels(channel_list);

    move(where);
}

void enter_game(int state)
{
    switch (state) {
    case 1:
	/* new user */
	if (wizardp(link)) {
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
	} else {
	    write("\n"
		  "Tuning in the newbie channel for you.  (newbie /on)\n"
		  "Tuning in the gossip channel for you.  (gossip /on)\n"
		  "\n");

	    /* these will be registered later */
	    channel_list = ({ "gossip", "newbie" });
	}
	/* FALLTHROUGH */   
    case 0:
	/* existing user */
	init_cmd_hook();
	CHANNEL_D->deliver_emote("announce", query_name(),
				 sprintf("enters %s.", mud_name()));
	/* move the body.  make sure this comes before the simple_action */
	if ( !move_to_start() ) {
	    write("Uh-oh, you have no environment.\n");
	} else {
	    /* we don't want other people to get the extra newlines */
	    write("\n");
	    if(is_visible())
		simple_action("$N $venter "+mud_name()+".");
	    write("\n");
	}
	CHANNEL_D->register_channels(channel_list);

	if ( wizardp(link) ) {
	    DID_D->dump_did_info(query_ilog_time(),
				 ({ "",
					"Changes since you last logged in",
					"********************************",
					"" }),
				 0,
				 (: enter_game, 2 :));
	    set_ilog_time(time());
	    return;
	}
	/* FALLTHROUGH */
    case 2:
	do_game_command("look");
    }
}

//:FUNCTION save_me
//Saves us :-)
void save_me()
{
    object shell_ob = link && link->query_shell_ob();
    string userid = query_userid();

    /* save the shell information */
    if ( shell_ob )
	shell_ob->save_me();

    //### This check is bogus.  What should it be?
    // This check also doesn't work for su's -- John
    //    if (previous_object()==this_object())
    saved_items = save_to_string(1); // 1 meaning it is recursive.

    unguarded( 1, (: save_object , USER_PATH(userid) :) );
    saved_items = 0;
}

//:FUNCTION remove
//Handle mailboxes and the last login daemon, as well as the normal stuff
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

//### This should be protected.
//:FUNCTION quit
//Quit the game.
void quit()
{
    if ( !clonep() )
    {
	::remove();
	return;
    }

    if (is_visible())
	simple_action("$N $vhave left "+mud_name()+".");

    CHANNEL_D->deliver_emote("announce", query_name(),
      sprintf("has left %s.", mud_name()));
    CHANNEL_D->unregister_channels();

#ifdef PLAYERS_START_WHERE_THEY_QUIT
    if (environment() && !wizardp(link))
	set_start_location(file_name(environment()));
#endif

    remove();
}

void do_receive(string msg, int msg_type) {
    if ( link )
	link->do_receive(msg, msg_type);
}

//:FUNCTION set_reply
//set_reply(s) sets the person to whom 'reply' goes to.
void set_reply(string o)
{
    reply = o;
}

//:FUNCTION query_reply
//query the person to whom reply goes to
string query_reply()
{
    return reply;
}

//:FUNCTION net_dead
//This function is called when we lose our link
void net_dead()
{
    //### add security here?

    if ( is_visible() )
	simple_action("$N $vhave gone link-dead.");

    CHANNEL_D->deliver_emote("announce", query_name(),
			     sprintf("has gone link-dead.", mud_name()));
}

//:FUNCTION reconnect
//This function is called when we get our link back
void reconnect(object new_link)
{
    //### add security here?

    link = new_link;
    if(is_visible())
	simple_action("$N $vhave reconnected.");

    CHANNEL_D->deliver_emote("announce", query_name(),
			     sprintf("has reconnected.", mud_name()));
}

//:FUNCTION die
//This function is called when we die :-)
void die()
{
    if ( wizardp(link) )
    {
	if(is_visible())
	    simple_action("If $n $vwere mortal, $n would now no longer be mortal.");
        set_hp(query_max_hp());
	stop_fight();
	return;
    }

    set_hp(0);
    if(is_visible())
	simple_action("$N $vhave kicked the bucket, and $vare now pushing up the daisies.");
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
    tell( bodies() - ({ this_body() }), msg );
    }
#endif
}

int clean_up() {
    return NEVER_AGAIN;
}

//:FUNCTION id
//id(s) returns 1 if we respond to the name 's'
int id(string arg)
{
    if(!is_visible() && arg == lower_case(query_invis_name()))
	return 1;

    return ::id(arg);
}

string stat_me()
{
    string result = short() + "\n" +
	"Userid: " + query_userid() + "\n" +
	::stat_me();

    if ( link )
	result += link->stat_me();

    return result;
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
    ** Make some of the flags non-persistent (they won't be saved).
    */
    configure_set(PLAYER_NP_FLAGS, 1);

    set_long( (: our_description :) );
    naming_create(userid);

    unguarded(1, (: restore_object, USER_PATH(userid), 1 :));

    // up to the player
    set_attack_speed(0);
}


/*
** Listen to channel messages and manipulate the channels
*/
void channel_rcv_string(string channel_name, string message)
{
    receive_private_msg(message);
}

void channel_rcv_soul(string channel_name, array data)
{
    string msg;

    if ( data[0][0] == this_object() )
	msg = data[1][0];
    else if ( sizeof(data[0]) == 2 && data[0][1] != this_object() )
	msg = data[1][2];
    else
	msg = data[1][1];

    receive_private_msg(msg);
}

void channel_add(string which_channel)
{
    channel_list += ({ which_channel });
    CHANNEL_D->register_channels(({ which_channel }));
}
void channel_remove(string which_channel)
{
    channel_list -= ({ which_channel });
    CHANNEL_D->unregister_channels(({ which_channel }));
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


/* verb interaction */
mixed indirect_give_obj_to_liv(object ob, object liv) {
    return 1;
}

int go_somewhere(string arg)
{
    object env = environment( this_object());
    int    ret;

    if(!(ret=::go_somewhere(arg)) && (env))
	return env->do_go_somewhere(arg);

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


void force_look()
{
    environment(this_object())->do_looking(1,this_object());
}

// Called when our environment destructs.  If we don't move, we get dested too.
void move_or_destruct(object suggested_dest) {
    mixed err;
    object dested_env = environment();
    mixed destination;

    if ( !query_link() )
	return;

    // Might want to add another failsafe room on the end of this list
    // that doesn't inherit room.c and is guaranteed to load/accept people.
    foreach (destination in ({ suggested_dest, VOID_ROOM, this_body()->query_start_location(), START, WIZARD_START  })) {
	err = catch {
	    if (stringp(destination))
		destination = load_object(destination);
	    if (destination != dested_env) {
		err = move(destination);
		if (stringp(err))
		    throw(err);
	    } else
		throw("Being destructed.\n");
	};
	if (destination && !err) {
	    receive_private_msg(dested_env->short() + " being destructed: You have been moved to " + destination->short() + ".\n");
	    return;
	} else {
	    if (destination)
		receive_private_msg("Cannot move to " + destination->short() + ": " + err);
	}
    }
    receive_private_msg("Uh oh..., couldn't move you anywhere.  Goodbye.\n");
    (this_object()->query_link())->remove();
}


#ifdef USE_STATS

int to_hit_base() {
    return 50 - query_agi();
}

int damage_bonus() {
    return fuzzy_divide(query_str(), 10);
}

void refresh_stats() {
    m_bodystats::refresh_stats();
}

#endif /* USE_STATS */

#ifdef USE_SKILLS

int hit_skill()
{
//### change to something based on skill...
    return fuzzy_divide(query_agi(), 2);
}

#endif /* USE_SKILLS */

/*
** These are overrides from our ancestor (MONSTER)
*/
string query_name() { return naming::query_name(); }
string short() { return query_name(); }
string a_short() { return query_name(); }
string the_short() { return query_name(); }
string in_room_desc() { return base_in_room_desc() + query_idle_string(); }

/* end of naming overrides */

#ifdef USE_SKILLS

class combat_result array negotiate_result(class combat_result array result)
{
    result = (class combat_result array)::negotiate_result(result);

    /*
    ** See if we have disarmed the opponent by using our 'combat/disarm'
    ** skill.  Note that testing the skill will also train it.
    */
    //### where's a better place to put this?
    //### ack. we should be caching our aggregate_skill()
    //### what the heck is the opposing skill?
    //### ... for now, use 5000; eventually, use target's experience

    if ( test_skill("combat/disarm", 5000) )
    {
	result = ({ new(class combat_result,
	    special : RES_DISARM,
	    message : "!disarm") }) + result;
    }

    return result;
}

#endif /* USE_SKILLS */

int allow(string what)
{
    if ( this_body() == this_object() )
    {
	return 1;
    }
    return 0;
}
