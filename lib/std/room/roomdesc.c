/* Do not remove the headers from this file! see /USAGE for more info. */

#include <playerflags.h>

private static string remote_desc;
private static string array room_state = ({});
private static mapping room_state_extra_longs = ([]);
private static int this_look_is_forced;

int query_light();
string short();
string show_exits();
string get_base_long();
string get_extra_long();

string array get_room_state_info()
{
  return copy(room_state);
}

string get_state_specific_long()
{
  string retval = "";
  mixed s;

  foreach(string state in room_state)
    {
      if(s = room_state_extra_longs[state])
	retval += evaluate(s)+"\n";
    }
  if(retval == "")
    return "\n";
  return retval;
}

int query_state(string state) {
    return member_array(state + "_on", room_state) != -1;
}

void set_room_state(string state)
{
  room_state -= ({state+"_off",state+"_on"});
  room_state += ({state+"_on"});
}

void clear_room_state(string state)
{
  room_state -= ({state+"_on",state+"_off"});
  room_state += ({state+"_off"});
}

void set_state_description(string state, mixed desc)
{
  string state_name;
  if((strlen(state) < 4) || ((state[<3..] != "_on")
			     && (state[<4..] != "_off")))
    {
      error("State description must be state + _on or _off\n");
    }
  if(state[<3] == '_')
    {
      if((member_array(state, room_state) == -1) &&
	 (member_array(state[0..<4]+"_off",room_state) == -1))
	clear_room_state(state[0..<4]);
    }
  else
    {
      if((member_array(state, room_state) == -1) &&
	 (member_array(state[0..<5]+"_on",room_state) == -1))
	clear_room_state(state[0..<5]);
    }
  room_state_extra_longs[state] = desc;
}

//:FUNCTION show_objects
//Return a string describing the objects in the room
varargs string show_objects(object except)
{
    object *obs;
    string user_show;
    string obj_show;
    string str;
    int n;
    object link;

    obs = filter(all_inventory() - ({ this_body() }), 
		 (: $1->is_visible() :));
    if(except)
      {
	obs -= ({except});
      }
    n = sizeof(obs);
    user_show = "";
    obj_show = "";

    while (n--) {
	if (obs[n]->is_living()) {
	  if((link = obs[n]->query_link()) && userp(link))
	    {
	      user_show += obs[n]->in_room_desc() + "\n";
	      continue;
	    }
	  str = obs[n]->in_room_desc();
	  if(strlen(str)) 
	    {
	      if(except)
		{
		  str += sprintf(" (outside %s)", except->the_short());
		}
	      obj_show += str + "\n";
	    }
	} else {
	    if (!duplicatep(obs[n])) {
		if ((str = obs[n]->show_in_room()) && strlen(str)) {
		  if(except)
		{
		  str += sprintf(" (outside %s)", except->the_short());
		}
		    obj_show += str + "\n";
		}
		if (obs[n]->inventory_visible())
		    obj_show += obs[n]->show_contents();
	    }
	}
    }
    if(except) // We're inside an object
      obj_show += except->inventory_recurse(0,this_body());

    if (user_show != "") obj_show += "\n"+user_show;
    return wrap(obj_show);
}

private int dont_show_long()
{
  return !this_look_is_forced && this_body()->test_flag(F_BRIEF);
}


string long()
{
#ifdef OBVIOUS_EXITS_BOTTOM
    return sprintf("%s%sObvious Exits: %s\n%s",
                   (dont_show_long() ? "" : get_base_long()[0..<2]),
		   get_state_specific_long(),
		   get_extra_long(),
                   show_exits(),
                   show_objects());
#else
    return sprintf("%s%s%s%s",
                   (dont_show_long() ? "" : get_base_long()[0..<2]),
		   get_state_specific_long(),
		   get_extra_long(),
		   show_objects());
#endif
}

//:FUNCTION long_without_object
//This is used by things like furniture, so the furniture can use the
//same long as the room, but not see itself in the description.
string long_without_object(object o)
{
#ifdef OBVIOUS_EXITS_BOTTOM
    return sprintf("%s%sObvious Exits: %s\n%s",
                   get_base_long()[0..<2],
		   get_state_specific_long(),
		   get_extra_long(),
                   show_exits(),
                   show_objects(o));
#else
    return sprintf("%s%s%s%s",
		   get_base_long()[0..<2],
		   get_state_specific_long(),
		   get_extra_long(),
		   show_objects(o));
#endif
  
}

//:FUNCTION do_looking
//print out the description of the current room
void do_looking(int forced_look)
{
    // This probably shouldn't be done in a global variable,
    // instead it should be passed to long(), but long does
    // not take args anywhere else...
    this_look_is_forced = forced_look;

    if ( wizardp(this_body()) &&
	 this_body()->query_shell_ob()->get_variable("show_loc") )
    {
	printf("[%s]\n", file_name(this_object()));
    }

    if ( query_light() < 1 )
    {
	write("Someplace dark\nIt is dark here.\n");
#ifdef ZORKMUD
	write("You might get eaten by a grue.\n");
#endif
    }
    else
    {
#ifdef OBVIOUS_EXITS
	printf("%s [exits: %s]\n", short(), show_exits());
#else
	printf("%s\n", short());
#endif

	write(wrap(long()));
    }
}




//### I think this should be torched :-)
// I don't.
// This should be overloaded if you want to be able to give different
// descs from different rooms
// Hmm, I know who made each of these comments.  Who else does? :-)
// I do =)
// Your mom.
void remote_look(object o)
{
    if ( remote_desc )
    {
	printf("%s\n", remote_desc);
    }
    else
    {
	printf("You can't seem to make out anything.\n");
    }
}

void set_remote_desc(string s)
{
    remote_desc = s;
}



