/* Do not remove the headers from this file! see /USAGE for more info. */

/*****************************************************

   inheritable to cause your monster to wander.
   where  the monster wanders to is controlled by 
   the set_area() and get_area() functions in room.

The important functions to call in your monster are
  set_movement(), and set_wander_area()


  writte by zifnab@lima bean 2/96
*****************************************************/



private string *obvious_exits;
private string enter_msg = "$N enters.\n";
private string exit_msg = "$N $vleave %s.";
private string array wander_area;

private int movement_time = 20;


//:FUNCTION set_movement_time()
//This function starts the first call_out for your monster
//if this is not called by your monster, then he will not 
// move.  arg passed in id the number of heart beats between
//  moves

void set_movement_time(int arg)
{
    movement_time = arg;
    call_out("move_this_monster", movement_time);
}

//:FUNCTION set_enter_message()
// This function takes a string as an arg that later
// gets passed to your enter message.
void set_enter_message(string enter)
{
    enter_msg = enter;
    return;
}

//:FUNCTION set_exit_message()
// sets the exit messsage for the monster
void set_exit_message(string exit)
{
    exit_msg = exit;
    return;
}
//:FUNCTION set_wander_area()
//This must be called by your monster so that he knows
//what area(s) to wander in.  this must match one of the set_area()
// in the rooms.
void set_wander_area(string array area...)
{
    wander_area = area;
}


//:FUNCTION move_this_monster()
//handles the movement of wandering monsters
mixed move_this_monster(int cloned)
{
    int i;
    int random_exit;
    string my_exit;
    string monster_exit;
    string array new_area;
    object ob;

    if(!environment(this_object()))
    {
	return;
    }
    else
    {
	obvious_exits = environment(this_object())->query_exit_directions(0);
	i = sizeof(obvious_exits);
	if(!i) {return;}
	random_exit = random(i);
	my_exit = obvious_exits[random_exit];
	monster_exit = environment(this_object())->query_exit_destination(my_exit);

#ifdef DEBUG
	write(my_exit); write(monster_exit);
#endif
	if(ob = load_object(monster_exit))
	{
	    if (new_area = ob->query_area())
	    {
	        foreach(string wa in wander_area)
	        {
		    if (member_array(wa, new_area) != -1)
		    {
		        this_object()->other_action(sprintf(exit_msg, my_exit));
		        this_object()->move(monster_exit);
		        this_object()->other_action(enter_msg);
		        break;
		    }
		}
	    }
	}
	call_out("move_this_monster", movement_time);

	return;
    }
}

