/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** smartmove.c
**
** A smarter movement system for some objects.  It handles movement from
** room to room.
*/

#include <move.h>
#include <playerflags.h>
#include <hooks.h>
#include <size.h>

varargs mixed move(mixed dest, string where);
string *get_player_message(string message, mixed arg);
int test_flag(int which);
void simple_action(string s);
varargs string compose_message(object forwhom, string msg, object *who, 
			       array obs...);


private nomask int move_me_there(string dest, string arg)
{
    object last_loc = environment();
    object env;
    object d;
    mixed  r;
    mixed txt;
    string *msgs;
    string m;

    if ( (r = move(dest)) != MOVE_OK)
    {
	if(!stringp(r)) {
	    if(r == MOVE_NO_ERROR) 
		return 1;

	    write("You remain where you are.\n");
	    return 0;
	}

	switch(r) {
	case MOVE_NO_DEST:
	    write("Construction blocks your path.\n");
	    return 0;
	case MOVE_NO_ROOM:
	    d = load_object(dest);
	    if(d->query_max_capacity()-d->query_capacity()-VERY_LARGE < 0) {
		if(sizeof(filter(all_inventory(d),(:$1->is_living():)))) {
		    write("You might be able to fit if there weren't "
		      "something moving around there already.\n");
		} else {
		    write("You're unable to fit.\n");
		}
	    } else {
		write("You aren't able to fit with the load you're "
		  "carrying.\n");
	    }
	    return 0;
	default:
	    write("You remain where you are.\n");
	    return 0;
	}
    }

    if ( !arg )
	arg = "somewhere";

    env = environment();

    txt = last_loc->query_exit_msg(arg);

    // This should be done better when smartmove gets cleaned up.  Right now
    // Furniture uses it, and anything else inheriting NON_ROOM probably does.
    if(!txt)
	txt = environment()->handle_exit_msgs(last_loc);
    if ( !txt )
    {
	msgs = get_player_message("leave", arg);

	// filter the object where you're going
	tell_from_inside(last_loc, msgs[1], 0, ({ env }));
    }
    else if ( arrayp(txt) )
    {
	if ( txt[0] )
	{
	    m = compose_message(this_object(), txt[0], ({ this_object() }));
	    tell(this_object(), m);
	}
	if ( txt[1] )
	{
	    m = compose_message(0, txt[1], ({ this_object() }));

	    // filter the object where you're going
	    tell_from_inside(last_loc, m, 0, ({ env }));
	}
    }
    else
    {
	simple_action(txt);
    }

    //### should we be adding this extra line? I think so...
    write("\n");

    //###there is a note in room.c about this being bogus
    txt = env->query_enter_msg(arg);
    if ( !txt )
    {
	msgs = get_player_message("enter", arg);
	tell_from_inside(env, msgs[1], 0, ({ this_object() }));
    }
    else if ( arrayp(txt) )
    {
	if ( txt[0] )
	{
	    m = compose_message(this_object(), txt[0], ({ this_object() }));
	    tell(this_object(), m);
	}
	if ( txt[1] )
	{
	    m = compose_message(0, txt[1], ({ this_object() }));
	    tell_from_inside(env, m, 0, ({ this_object() }));
	}
    }
    else
    {
	simple_action(txt);
    }

    return r == MOVE_OK;
}

//:FUNCTION notify_move
//Basically do a this_body()->force_look().
//Seperated out so vehicles could change this
//without having to override the move_to() function.

void notify_move()
{
    this_body()->force_look(0);
}


//:FUNCTION move_to
//Move the object to a new location.  Returns 1 if the move was successful,
//0 otherwise.  The first arg is a filename, the second is the direction
//that caused the movement to that destination.

//:HOOK person_left
//Called when a person successfully leaves a room in a certain direction
//(called by the room).  The return value is ignored.  The person moving
//is given by this_body().  The direction is passed as an argument.
int move_to(string dest, mixed dir)
{
    object where = environment();

    dest = evaluate(dest);
    if (move_me_there(dest, dir)) {
	where->call_hooks("person_left", HOOK_IGNORE, 0, dir);

    }

    if ( where != environment() )
    {
	this_object()->notify_move();
	return 1;
    }
    return 0;
}

int go_somewhere(string arg)
{
    object env = environment( this_object());
    string value = env->query_exit_value(arg);


    // This should have been trapped already, but just in case:
    if( !value )
	return call_other( env, "do_go_" + arg );
    if (value[0] == '#') {
	write(value[1..]);
	return 0;
    }

    if (env->call_hooks("block_" + arg, HOOK_LOR, 0, arg)
      || env->call_hooks("block_all", HOOK_LOR, 0, arg))
	return 0;

    return move_to(value, arg);
}

void do_go_somewhere( string arg )
{
    int ret = 0;
object env = environment( this_object());

    // allowed by the room itself; some sort of 'special' exit
    if( function_exists( "do_go_" + arg, env ))
	ret = call_other(env, "do_go_" + arg);
    if(!ret)
	go_somewhere( arg );
    return;
}
