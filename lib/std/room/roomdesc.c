/* Do not remove the headers from this file! see /USAGE for more info. */

#include <playerflags.h>


private nosave string remote_desc;

int query_light();
string short();
string show_exits();
string long();

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

  while (n--)
  {
    if (obs[n]->is_living())
    {
      str = obs[n]->in_room_desc();
      if((link = obs[n]->query_link()) && userp(link))
      {
        if(except)
		      str += sprintf(" (outside %s)", except->the_short());
        user_show += str + "\n";
        continue;
      }
      if(strlen(str)) 
      {
        if(except)
		      str += sprintf(" (outside %s)", except->the_short());
        obj_show += str + "\n";
      }
    } else {
      if (!duplicatep(obs[n]))
      {
        if ((str = obs[n]->show_in_room()) && strlen(str))
        {
		      if(except)
            str += sprintf(" (outside %s)", except->the_short());
          obj_show += str + "\n";
        }
        if(obs[n]->inventory_visible() && !obs[n]->query_hide_contents()) 
          obj_show += obs[n]->show_contents();
      }
    }
  }
  if(except) // We're inside an object
    obj_show += except->inventory_recurse(0,this_body());

  if(user_show != "")
  {
    if( obj_show != "") obj_show += "\n";
    obj_show += user_show;
  }
  return obj_show;
}

//### major hack
private nosave int this_look_is_forced;

protected int dont_show_long()
{
    return !this_look_is_forced && this_body()->test_flag(F_BRIEF);
}

//:FUNCTION do_looking
//print out the description of the current room
varargs void do_looking(int force_long_desc, object who)
{
    //### how to use force_long_desc ??

    if ( wizardp(who) && who->query_link()->query_shell_ob()->get_variable("show_loc") )
    {
	tell(who, sprintf("[%s]\n", file_name(this_object())));
    }

    if ( query_light() < 1 )
    {
	tell(who, "Someplace dark\nIt is dark here.\n");
#ifdef ZORKMUD
	tell(who, "You might get eaten by a grue.\n");
#endif
    }
    else
    {
#ifdef OBVIOUS_EXITS
	tell(who, sprintf("%%^ROOM_SHORT%%^%s%%^RESET%%^ [exits: %%^ROOM_EXIT%%^%s%%^RESET%%^]\n", short(), show_exits()));
#else
	tell(who, sprintf("%%^ROOM_SHORT%%^%s%%^RESET%%^\n", short()));
#endif

	//### for now, hack a global
	this_look_is_forced = force_long_desc;
	tell(who, long());
	this_look_is_forced = 0;
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


