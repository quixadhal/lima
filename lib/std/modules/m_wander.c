/* Do not remove the headers from this file! see /USAGE for more info. */

 /* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Monster wandering module.
 * This module will allow monsters to wander in designated area, with 
 * specified time intervals between movements.
 * Written by Tigran 03/27/98.
 */

//:MODULE
//
//The wander module designed to be included by NPC's which allows monsters
//to wander throughout various areas in the gaming area.

//:TODO
//Something to have the NPC's open doors or unlcok doors.  It is possible
//that this belongs in a separate module.

#define ZERO_RETURN 10

void add_hook(string tag, function hook);
void remove_hook(string tag, function hook);
void do_wander();
void do_game_command(string str);
int query_attacking();
int player_did_arrive(string dir);
void moving();

private mixed wander_time;
private string array wander_area=({});
private nosave int call;
private nosave int num_moves;
private int max_moves;
private nosave int counting_moves;
private nosave function arrive_func=(:player_did_arrive:);
private nosave function move_hook=(:moving:);

//:FUNCTION set_wander_area
//Set the area(s) that an NPC can wander in.  If this is not set 
//it is assumed that the NPC can wander anywhere without area
//restrictions.
void set_wander_area(string array area...)
{
  wander_area=clean_array(area);
}

//:FUNCTION add_wander_area
//Add area(s) which an NPC can wander in.  See set_wander_area()
void add_wander_area(string array area...)
{
  wander_area=clean_array(wander_area+({area}));
}

//:FUNCTION remove_wander_area
//Remove area(s) which an NPC can wander in.  See set_wander_area()
void remove_wander_area(string array area...)
{
  if(member_array(area,wander_area))
    wander_area-=({area});
}

//:FUNCTION clear_wander_area
//Clear the area(s) in which an NPC can wander in.  Effectively
//this allows the NPC to wander anywhere.  See set_wander_area()
void clear_wander_area()
{
  wander_area=({});
}

//:FUNCTION query_wander_area
//Returns an array of areas in which may wander.  
//See set_wander_area()
string array query_wander_area()
{
  return wander_area;
}

//:FUNCTION set_wander_time
//Sets the time between an NPC's movements.  
//
//If the argument is an int it sets the time in seconds.
//
//If the argument is a function pointer, the function 
//needs to evaluate to an int.  The fpointer is evaluated
//during runtime.
void set_wander_time(mixed time)

{
  if(!intp(time)&&!functionp(time))
    return;
  wander_time=time;
}

//:FUNCTION query_wander_time
//Returns an integer which is the amount of time to be used
//for the interval between movements.  It does not return 
//the time til the next movement.  query_wander_time()
//will not return 0, if the return is 0 or less 
//ZERO_RETURN (refer to the M_WANDER file) will be returned
int query_wander_time()
{
  int i;
  if(intp(wander_time))
    i=wander_time;
  else
    i=evaluate(wander_time);
  if(i<2)
    return ZERO_RETURN;
  return i;
}

//:FUNCTION set_max_moves
//Sets the maximum number of moves a monster will make without
//running into a player.  This prevents too many NPCs
//moving around for no reason.  If the argument is 0 it assumes
//that the NPC will not stop moving, regardless of player 
//interaction
void set_max_moves(int i)
{
  max_moves=i;
}

//:FUNCTION query_max_moves
//Returns the number of moves an NPC will make without player 
//interaction before stopping.
int query_max_moves()
{
  return max_moves;
}

private void reset_moves()
{
  num_moves=0;
  //counting_moves=0;
}

private void start_move_count()
{
  if(!max_moves)
    return;
  counting_moves=1;
}

//:FUNCTION cease_wandering
//Stops an NPC from wandering.  
//start_wandering() is required to make the NPC move again
void cease_wandering()
{
  remove_call_out(call);
  call=0;
  environment()->remove_hook("person_arrived",arrive_func);
}


//:FUNCTION stop_wandering
//Stops an NPC from wandering.  
//If you use this function the npc will start moving again if a PC enters the 
//room.
void stop_wandering()
{
  remove_call_out(call);
  call=0;
  environment()->add_hook("person_arrived",arrive_func);
}

//:FUNCTION start_wandering
//Starts an NPC wandering
void start_wandering()
{
  if(call)
    return;
  if(!environment())
    return;
  environment()->add_hook("person_arrived",arrive_func);
  call=call_out( (: do_wander :), query_wander_time() );
  reset_moves();
}

/* Hook function that is called after we move into the new location
 * Check here to see if there are other PC's in the room.  If so reset the 
 * count and continue moving. 
 */
void moving()
{
  environment()->add_hook("person_arrived",arrive_func);
  foreach(object ob in all_inventory(environment()))
    {
      if(ob->query_link())
	{
	  reset_moves();
	  break;
	}
    }
}

int player_did_arrive(string direction)
{
  reset_moves();
  if(this_body()->query_link())
    start_wandering();
  return 1;
}

private void move_me(string direction)
{
  environment()->remove_hook("person_arrived",arrive_func);
  do_game_command(sprintf("go %s",direction));
  if(!counting_moves)
    start_move_count();
  num_moves++;

  if(counting_moves && 
     (num_moves>=max_moves))
    {
      stop_wandering();
      return;
    }
  call=call_out( (: do_wander :), query_wander_time());
}

private void do_wander()
{
  string array directions;
  int i;
  string chosen_dir;
  string file;
  object dest;
  
  if(environment(this_object()))
    directions=environment(this_object())->query_exit_directions();
  /* Stop if there are no directions (this takes care of blue prints*/
  if(!directions||!i=sizeof(directions))
    return;

  chosen_dir=directions[random(i)];
  file=environment(this_object())->query_exit_destination(chosen_dir);
  /* If the file is null or a null string try to move again next time */
  if(!file||file=="")
    {
      call=call_out( (: do_wander :), query_wander_time());
      return;
    }
  /* If the destination is not loaded, do so */
  dest=find_object(file);

  if(!dest)
    dest=load_object(file);

  /* If still no destination it won't load, try again next time */
  if(!dest)
    {
      call=call_out( (: do_wander :), query_wander_time());
      return;
    }
  /* Check if the npc has wander restrictions */
  if(sizeof(wander_area))
    {
      if(arrayp(wander_area)&&
	 arrayp(dest->query_area()))
	{
	  if(sizeof(wander_area&dest->query_area()))
	    move_me(chosen_dir);
	  else
	    call=call_out( (: do_wander :), query_wander_time());
	}
      else
	call=call_out( (: do_wander :), query_wander_time());
    }
  else
    move_me(chosen_dir);
  return;
}

void mudlib_setup()
{
  add_hook("move",move_hook);
}

mapping lpscript_attributes()
{
  return (["wander_area" : ({ LPSCRIPT_LIST, "setup", "set_wander_area" }),
	   "wander_time" : ({ LPSCRIPT_INT, "setup", "set_wander_time" }),
	   "max_moves" : ({ LPSCRIPT_INT, "setup", "set_max_moves" }) ]);
}
