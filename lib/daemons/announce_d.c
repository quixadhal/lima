/* Do not remove the headers from this file! see /USAGE for more info. */

#include <ansi.h>
#include <playerflags.h>
#include <mudlib.h>

inherit DAEMON;
inherit M_MESSAGES;

#ifdef ZORKMUD
#define WORLD_NAME	"The Great Underground Empire"
#else
#define WORLD_NAME	mud_name()
#endif


void announce_me(int type){
    object* u;
    int i;
    int hide;
    string *msgs;
    object *who;
    string name;

//    if(file_name(previous_object())[0..4] != "/std/") return;
    hide = previous_object()->query_hidden();

    u = filter_array(all_users(), (: $1->query_body() &&
				   $1->query_body()->test_flag(F_ANNOUNCE) &&
				   $1->query_body()->query_level() > $(hide) :) );
    u -= ({ previous_object() });
    who = ({ previous_object() });
    switch(type){
    case 1:
        msgs = action( who, "($N $vhave entered "+WORLD_NAME+".)\n");
	break;
    case 2:
        msgs = action( who, "($N $vhave left "+WORLD_NAME+".)\n");
	break;
    case 3:
        msgs = action( who, "($N $vhave reconnected.)\n");
	break;
    case 4:
        msgs = action( who, "($N $vhave gone link-dead.)\n");
	break;
    default:
	return;
    }
    map_array(u, (: tell_object($1, $(msgs[<1])) :));
}

