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
string array action(object array x, string s);
void simple_action(string s);


private nomask int move_me_there(string dest, string arg, object last_loc)
{
    object env = environment();
    object d;
    mixed  r;
    string txt;
    string *msgs;

    if ( (r = move(dest)) != MOVE_OK)
    {
      dest = evaluate_path(
	 (strlen(dest) && dest[0] == '/' ? "" : env->query_base()) + dest);
      r = move(dest);
	if (r != MOVE_OK)
	{
	  if(!stringp(r))
	    {
	      if(r == MOVE_NO_ERROR)
		{
		  return 1;
		}
	      write("You remain where you are.\n");
	      return 0;
	    }
	      
	  switch(r)
	    {
	    case MOVE_NO_DEST:
	      if(stringp((env->get_exits())[arg]))
		{
		  write("Construction blocks your path.\n");
		}
	      return 0;
	    case MOVE_NO_ROOM:
	      d = load_object(dest);
	      if(d->query_max_capacity()-d->query_capacity()-VERY_LARGE < 0)
		{
		  if(sizeof(filter(all_inventory(d),(:$1->is_living():))))
		    {
		      write(wrap("You might be able to fit if there weren't "
			    "something moving around there already.\n"));
		    }
		  else
		    {
		      write("You're unable to fit.\n");
		    }
		}
	      else
		{
		  write("You aren't able to fit with the load you're "
			"carrying.\n");
		}
	      return 0;
	    default:
	      write("You remain where you are.\n");
	      return 0;
	    }
	}
    }

//### bogus place to show the path... probably should be handled in
//### another way
    if ( test_flag(F_DISPLAY_PATH) )
    {
	printf("[ %s ]\n", file_name(env));
    }

    if ( !arg )
	arg = "somewhere";

//###this came from room.c, but there was no way to set it... what is it?
//###seems like custom exit messages... needed?  if so, query room for it
    txt = last_loc->query_exit_msg(arg);
    if ( !txt )
    {
	msgs = get_player_message("leave", arg);
	tell_room(env, msgs[1], ({ this_object() }));
    }
    else
    {
      
      msgs = action(({this_object()}), txt);
      tell_object(this_object(),msgs[0]);
      tell_room(last_loc, msgs[1]);
    }

    env = environment();

//###there is a note in room.c about this being bogus
    txt = env->query_enter_msg(arg);
    if ( !txt )
    {
	msgs = get_player_message("enter", arg);
	tell_room(env, msgs[1], ({ this_object() }));
    }
    else
    {
      simple_action(txt);
    }

    return r == MOVE_OK;
}

/*
** really funky destination evaluation code.
** Read the lfun docs under "exits".
*/
int evaluate_destination(mixed dest, string arg, object old_loc)
{
    int i,n;
    mixed foo,tmp;

    if (pointerp(dest)) {
	i = 0;
	n = sizeof(dest);
	while (i<n) {
	    foo = dest[i]; /* optimization */
	    if (intp(foo) && !foo) return 0;
	    if (intp(foo)) i+=foo;
	    if (stringp(foo)) {
		if (move_me_there(foo, arg, old_loc)) return 1;
		i++;
	    }
	    if (functionp(foo)) {
		tmp = evaluate(foo);
		if (!tmp) return 0;
		if (stringp(tmp)) {
		    if (move_me_there(tmp, arg, old_loc)) return 1;
		    i++;
		} else i+=tmp;
	    }
	}
	return 1;
    } else {
	return move_me_there(dest, arg, old_loc);
    }
}

//:FUNCTION move_to
//Move the object to a new location.  Returns 1 if the move was successful,
//0 otherwise.  One of the funky destination values should be passed along
//with what direction that caused the movement to that destination.
int move_to(mixed dest, mixed dir)
{
    mixed ret;
    object where = environment();

    dest = evaluate(dest);

//### I don't think the value 0 is right here
    if ( (ret = evaluate_destination(dest, dir, where)) != 0 )
    {
//### correct to call only if the current body? maybe also call for non-bodies
	if ( this_object() == this_body() )
	{
//:HOOK person_left
//Called when a person successfully leaves a room in a certain direction
//(called by the room).  The return value is ignored.  The person moving
//is given by this_body().  The direction is passed as an argument.
	    where->call_hooks("person_left", HOOK_IGNORE, 0, dir);
	}
    }

    if ( where != environment() )
	force_look();

    return ret;
}

int go_somewhere(string arg)
{
    object env = environment();
    string dest = env->get_exits()[arg];

    if ( !dest ) {
	if (dest = env->get_default_exit()) {
	    string ret;
	    ret = evaluate(dest);
	    if (stringp(ret))
		write(ret);
	}
// I don't think this ever gets called...
// db: it "shouldn't"...
	else
	    write(wrap("There are tales of mighty wizards who walk "
		       "through walls, but unfortunately\nthese are "
		       "but tales without foundation.\n"));
	return 0;
    }

    return move_to(dest, arg);
}
