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

#define USE_TITLES


// Files we need to inherit --
inherit MONSTER;
//inherit M_ACCESS;
inherit M_GRAMMAR;
//inherit M_INPUT;

inherit "/std/player/quests";
//inherit "/std/player/alias";
inherit "/std/player/mailbase";
inherit "/std/player/newsdata";
inherit "/std/player/path";
inherit "/std/player/wizlevel";
inherit "/std/player/cmd";
inherit "/std/player/help";
inherit "/std/player/bodyshell";

#ifdef USE_TITLES
inherit "/std/player/title";
#endif

// Global variables --

string name = "guest";
string describe;
string invis_name;
static object link;
static string cap_name;
string start_location;
string reply;
mapping nickname_list;
int more_chunk;

private string * channel_list;

// part of patch for handling auto loading  -- Pere@ZorkMUD
string* auto_load;

// login related stuff --

int init_user(string str){
    cap_name = capitalize(str);
    name = str;
    restore_object(USER_PATH(str), 1);
    if (!describe)
    {
	describe = ( capitalize(name) +
		    " is a boring player and hasn't described " +
		    query_reflexive() + ".\n"
		    );
    }

    return 1;
}

/* this is so that players don't show up as (wielded) */
string extra_short() { return 0; }

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

#if 0
    if (GROUP_D->adminp(this_object()))
	set_privilege(1);
    else
	set_privilege(name);
#endif

    link = previous_object();
//    load_outside_aliases();

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

    add_id( name );

    TIMEOUT_D->create();
    ANNOUNCE_D->announce_me(1);
    NCHANNEL_D->register_channels(channel_list);

    set_mass(100);
    set_max_capacity(100);

    bodyshell::start_shell();
    load_autoload();            
}


void enter_game(int is_new)
{
    init_cmd_hook();
    if(move(start_location) && move(START)){
	write("Uh-oh, you have no environment.\n");
	return;
    }
    if(is_new)
      {
	write("Tuning in the newbie channel for you.  (newbie /on)\n");
	do_game_command("chan newbie /on");
	write("Tuning in the gossip channel for you.  (gossip /on)\n");
	do_game_command("chan gossip /on");
      }
    simple_action("$N $venter the game.\n");
    do_game_command("look");
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
    save_me();
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
    if (!test_flag(INVIS))
	simple_action("$N $vhave left the game.\n");
    save_autoload();
    ANNOUNCE_D->announce_me(2);
    NCHANNEL_D->unregister_all();
    if (environment() && !wizardp(link))
	start_location = file_name(environment());
    remove();
}


string in_room_desc()
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

    /* titles may include a %s in them. substitute their name */
    return punctuate(sprintf(result, cap_name));
}


void set_reply(string o){
    reply = o;
}

string query_reply(){ return reply; }

string add_default_message(string message) {
    string str;
    switch (message) {
    case "leave": str = "$N $vleave $o\n"; break;
    case "mleave": str = "$N $vdisappear in a puff of smoke.\n"; break;
    case "enter": str = "$N $venter.\n"; break;
    case "menter": str = "$N $vappear in a puff of smoke.\n"; break;
    case "invis": str = "$N $vfade from view.\n"; break;
    case "vis": str = "$N $vfade into view.\n"; break;
    case "home": str = "$N $vgo home.\n"; break;
    case "clone": str = "$N $vclone the $o.\n"; break;
    case "destruct": str = "$N $vdest the $o.\n"; break;
    default: str = "$N $vdo not have a message for '"+message+"'.\n";
    }
    messages[message] = str;
    return str;
}

void do_player_message(string message, mixed arg) {
    mixed mess;
    mixed foo;

    if(!mapp(messages))
	messages = ([]);
    mess = messages[message];
    if (!mess) mess = add_default_message(message);
    if (pointerp(mess)) mess = mess[random(sizeof(mess))];

    simple_action(mess, arg);
}

string *get_player_message(string message, mixed arg) {
    mixed mess;
    mixed foo;

    mess = messages[message];
    if (!mess) mess = add_default_message(message);
    if (pointerp(mess)) mess = mess[random(sizeof(mess))];

    return action( ({ this_object() }), mess, arg);
}

void net_dead()
{
    simple_action("$N has gone link-dead.\n");
    ANNOUNCE_D->announce_me(4);
    call_out("remove",300);
}

void reconnect()
{
    link = previous_object();
    remove_call_out("remove");
    simple_action("$N $vhave reconnected.\n");
    bodyshell::restart_shell();
//    modal_push((: parse_line :), (: "> " :));
    ANNOUNCE_D->announce_me(3);
}



mixed query_who_data()
{
    return ({ get_flags(PLAYER_FLAGS), query_real_name(), short(), query_level() });
}




int query_hidden(){
    if(test_flag(F_HIDDEN)) return query_level();
}

void die(){
    simple_action("$N $vhave kicked the bucket, and $vare now pushing up the daisies.\n");
    tell_object(this_object(), "   ****  You have died  ****\n"
      "A pity, really.  Way too many people dying these days for me to just patch\n"
      "everyone up.  Oh well, you'll live.\n");
    rack_up_a_death();
    link->switch_body("/std/ghost");
}


void clean_up()
{
    remove();
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
    write("Name: "+cap_name+" ("+query_level()+")\n");
    ::stat_me();
    if ( link )
	link->stat_me();
    return 1;
}

void add_to_nickname_list( string who, string nick )
{
    if( nickname_list[who] )
    {
	nickname_list[who] = ({ nick });
	return;
    }

    nickname_list[who] += ({nick});
}

void del_from_nickname_list( string who, string which )
{
    if( which && sizeof( nickname_list[who] ) > 1 )
    {
	nickname_list[who] -= ({ which });
	return;
    }
    map_delete( nickname_list, who );
}

mapping query_nickname_list()
{
    return copy( nickname_list );
}

void set_description(string str){
  if(base_name(previous_object()) == CMD_OB_DESCRIBE)
    describe = str;
  save_me();
}

string our_description() {
    return in_room_desc() + "\n" + describe +"\n";
}

void create()
{
    auto_load = ({});
    nickname_list = ([]);
    messages = ([]);
    channel_list = ({ });

    monster::create();
//    parser::create();
//    path_create();
    
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

