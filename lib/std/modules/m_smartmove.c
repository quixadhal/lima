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
#include <classes.h>

inherit CLASS_MOVE;

varargs mixed move(mixed dest, string where);
string query_msg(string which);
int test_flag(int which);
void simple_action(string s,array obs... );
varargs string *action(object *who, mixed msg, array obs...);
varargs string compose_message(object forwhom, string msg, object *who, 
  array obs...);
object query_target();


private nomask int move_me_there(class move_data data)
{
    object last_loc = environment();
    object env;
    object d;
    mixed  r;
    mixed txt;
    string m;

    if ( (r = move(data->destination,data->relation)) != MOVE_OK) {
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
	    d = load_object(data->destination);
	    if(d->query_max_capacity()-d->query_capacity()-VERY_LARGE < 0) {
		if(sizeof(filter(all_inventory(d),(:$1->is_living():)))) {
		    write("You might be able to fit if there weren't "
		      "something moving around there already.\n");
		} 
		else {
		    write("You're unable to fit.\n");
		}
	    }
	    else {
		write("You aren't able to fit with the load you're "
		  "carrying.\n");
	    }
	    return 0;
	default:
	    write("You remain where you are.\n");
	    return 0;
	}
    }
    /* Make sure that there is some type of direction for the exit */
    if ( !data->exit_dir ) {
	data->exit_dir= "somewhere";
    }
    /* Also ensure that there is a valid object being moved */
    if ( !data->who ) {
	data->who = this_body();
    }

    env = environment();

    /* Exit Messages */
    txt = data->exit_messages;

    /* If there is no exit message for the exit, use the default for that body.
     * The default message is not one that should be processed any further.*/
    if ( !txt )
	txt = query_msg("leave");

    /* Display the message */
    if(data->source)
	tell_from_inside(last_loc,action(({data->who}),txt,data->source)[1]);
    else
	tell_from_inside(last_loc,action(({data->who}),txt,data->exit_dir)[1]);

    /* Entrance messages */
    txt = data->enter_messages;

    /* If there is no enter message for the exit, use the default for that body.
     * The default message is not one that should be processed any further.*/
    if ( !txt )
	txt = query_msg("enter");

    /* Display the message */
    if(data->through)
	simple_action(txt,data->through);
    else if(data->source)
	simple_action(txt,data->source);
    else
	simple_action(txt);

    return r == MOVE_OK;
}

//:FUNCTION notify_move
//Basically do a this_body()->force_look().
//Seperated out so vehicles could change this
//without having to override the move_to() function.

void notify_move()
{
    this_object()->force_look(0);
}


//:FUNCTION move_to
//Move the object to a new location.  Returns 1 if the move was successful,
//0 otherwise.  The first arg is a filename, the second is the direction
//that caused the movement to that destination.

//:HOOK person_left
//Called when a person successfully leaves a room in a certain direction
//(called by the room).  The return value is ignored.  The person moving
//is given by this_body().  The direction is passed as an argument.

//:HOOK person_arrived
//Called when a person successfully enters a room from a direction.
//The return value is ignored. The person moving is given by this_body(). 
//The direction is passed as an argument.
//varargs int move_to(string dest, mixed dir, mixed exit, mixed enter)
varargs int move_to(class move_data data)
{
    object where = environment();
    if (move_me_there(data)) {
	where->call_hooks("person_left", HOOK_IGNORE, 0, data->exit_dir);
	environment()->call_hooks("person_arrived", HOOK_IGNORE, 0, data->exit_dir);
    }

    if ( where != environment() )
    {
	this_object()->notify_move();
	return 1;
    }
    return 0;
}
