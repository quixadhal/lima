/* Do not remove the headers from this file! see /USAGE for more info. */

// /std/dev.c  
// Rust@ZorkMUD
// with mods by alot of us:
// including:   Nevyn@ZorkMUD (GUE)
//              Peregrin @ZorkMUD (GUE)
//
#include <daemons.h>
#include <move.h>
#include <mudlib.h>
#include <config.h>
#include <security.h>
#include <setbit.h>

void set_ilog_time(int);

// Files we need to inherit --
inherit PLAYER;

// Global variables --

private string pwd = "/";
private string cwf;
private int ilog_time;

// login related stuff --

void enter_game(int is_new_wiz)
{
    string login_file;

    player::enter_game(0);

    // do .login stuff
    login_file = wiz_dir(this_object()) + "/.login";
    if( file_size( login_file ) > 0 )
	map_array( explode(read_file(login_file),"\n"),
	  (: this_user()->force_me($1) :) );
    
    if(!is_new_wiz)
      DID_D->dump_did_info(ilog_time);
    else
      {
	write("Hi, new wiz! Tuning you in to all the mud's important channels.\n");
	write("Doing: wiz /on\n");
	link->force_me("wchan wiz /on");
	write("Doing: chan news /on   (you'll see when new news is posted.)\n");
// for some reason aliases don't catch from a force_me from here, so use the
// full cmd. (probably a better idea anyway P-)
	link->force_me("chan news /on");
	write("Doing: gossip /on\n");
	link->force_me("chan gossip /on");
	write("Doing: newbie /on\n");
	link->force_me("chan newbie /on");
        write("Doing: announce /on\n");
        link->force_me("wchan announce /on");
      }
    set_ilog_time(time());
}


// caller verification here
void
ilog_hook(string str) {
    tell_object(this_object(), "*** " + str + "\n");
    set_ilog_time(time());
}

static void
set_ilog_time(int t)
{
  ilog_time = t;
}

void set_pwd(string arg){ 
    if(this_body() != this_object()) return;
    pwd = arg;
}

string query_pwd(){ return pwd; }

// caller verification here
void set_cwf( string arg){
    cwf = arg;
}

string query_cwf(){ return cwf; }

void die(){
    simple_action("If $N $vwere mortal, $n would now no longer be mortal.\n");
    set_hp(10);
}

int set_start_location( string str )
{
    if(!stringp(str))
	return 0;
    if(load_object(str)) 
    {
	start_location = str;
        return 1;
    }
    else
	return 0;
}

string query_start_location()
{
    if( start_location ) return start_location;
    return START;
}

string query_home()
{
    string home;

    home = wiz_dir(this_object()) + "/workroom";
    if ( file_size(home + ".c") > 0 )
	return home;

    return query_start_location();
}

nomask void wiz_switch_body(string obname)
{
    /* pass ob, not string -- a string permanently switches :-) */
    link->switch_body(new(obname));
}














