/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Monster following module
 * Designed to allow NPC's to follow other NPCs or PCs
 * 092998 Created by Tigran, based on /domains/std/wolf.c
 */

//:MODULE
//
// Module designed to allow objects to follow other moving objects.
// This module requires functionality from M_SMARTMOVE, M_TRIGGERS and 
// M_ACTIONS.

//:TODO
//
// 1.) Target selection could be made smarter than it is.
// 2.) Add a hook call to prevent following?

#include <hooks.h>

void follow_hook(string whichway);
void arrive_hook(object what);
void acquire_follow();
void call_hooks(string, int);
void respond(string str);
void add_hook(string tag, function hook);

private nosave function f_follow=(:follow_hook:);
private nosave function arrive=(:arrive_hook:);
private mixed array follow_search=({});
private object follow;
private nosave object my_location;
private nosave int following;

//:FUNCTION set_follow_search
//Set the names of the objects that the object will follow.
//Allowable arguments are strings, objects or function pointers.
//Strings must be an id of the object which you want to have 
//followed.
//If a function pointer is used it must return a string, object
//or an array of objects and strings.  An argument of this_object
//is passed to the function pointer.
void set_follow_search(mixed array follow...)
{
  follow_search=clean_array(flatten_array(follow));
}

//:FUNCTION add_follow_search
//Add names to the list of objects that the object will follow
//See set_follow_search
void add_follow_search(mixed array follow...)
{
  follow_search=clean_array(flatten_array(follow_search+follow));
}

//:FUNCTION remove_follow_search
//Remove names from the list of objects that the object will follow
void remove_follow_search(mixed array follow...)
{
  follow_search-=flatten_array(follow);
}

//:FUNCTION clear_follow_search
//Clears the search array for following
void clear_follow_search()
{
  follow_search=({});
}

varargs private mixed array expand_follows()
{
  mixed array potentials=({});
  foreach(mixed elem in follow_search)
    {
      if(functionp(elem))
	potentials+=({evaluate(elem,this_object())});
      else if(stringp(elem)||objectp(elem) )
	potentials+=({elem});
    }
  return clean_array(potentials);
}

//:FUNCTION query_follow_search
//Returns a list of the follow strings that the object will follow
string array query_follow_search()
{
  return expand_follows();
}

//:FUNCTION set_follow
//Set the follow that the object will follow
void set_follow(object what)
{
  if(what!=this_object())
    follow=what;
}

//:FUNCTION clear_follow
//Clear the follow that's the object will follow
void clear_follow()
{
  follow=0;
}

//:FUNCTION query_follow
//Returns the follow object which the object is currently following
object query_follow()
{
  return follow;
}

//:FUNCTION is_following
//Returns 1 if the object is currently following another object
int is_following()
{
  return !!following;
}

//:FUNCTION acquire_follow
//If there is no current follow, or the current follow is not present.
//The new follow is determined by the follow_search
//:TODO 
//perhaps add something to allow weighted choices for follows?
void acquire_follow()
{
  int i;
  mixed array potentials=({});
  if(follow && present(follow,environment(this_object() ) ) )
    return;
  
  potentials=filter(expand_follows(), (: present($1,environment(this_object())) :) );
  i=sizeof(potentials);
  if(i)
  {
    set_follow(present(potentials[random(i)],environment(this_object())));
  }
  else
    clear_follow();
}

private int follow_match(mixed check, object ob)
{
  if(functionp(check))
    return member_array(ob,eval(check,ob))>-1;
  if(objectp(check))
    return ob==check;
  if(stringp(check))
    return ob->id(check);
}

//:FUNCTION is_potential_follow
//Return 1 if the object is a potential follow to follow
int is_potential_follow(object ob)
{
  return sizeof(filter(expand_follows(),(:follow_match:),ob));
}

void follow_hook(string whichway)
{
  if(follow==this_body())
    {
      set_this_user(this_object());
      respond("go "+whichway);
      set_this_user(follow->query_link());
    }
}

void arrive_hook(object what)
{
  object env = environment(this_object());

  if(!env)
    return;

  if((follow && present(follow,env))|| query_follow()==what)
    return;
  clear_follow();
  if(is_potential_follow(what))
    set_follow(what);
}

protected void did_move() 
{
  if(!sizeof(follow_search))
    return;
  if (my_location)
    {
      my_location->remove_hook("person_left", f_follow);
      my_location->remove_hook("object_arrived",arrive);
    }
  my_location = environment();
  if (my_location)
    {
      my_location->add_hook("person_left", f_follow);
      my_location->add_hook("object_arrived",arrive);
    }
  //: HOOK check_follow
  //This hook allows the follow and/or follow_searches to be changed
  //due to changing environmental conditions, aggression, whatever.
  //Return value is ignored.
  call_hooks("check_follow",HOOK_IGNORE);
  acquire_follow();
}

void mudlib_setup()
{
  add_hook("move",(: did_move :));
}

mapping lpscript_attributes()
{
  return (["follow_search":({LPSCRIPT_LIST,"setup","set_follow_search"}) ]);
}

void do_follow_obj(object ob)
{
  if(member_array(ob,follow_search)>-1)
  {
    remove_follow_search(ob);
    clear_follow();
    this_body()->my_action("You stop following "+ob->short()+".");
  }
  else
  {
    add_follow_search(ob);
    this_body()->my_action("You begin following "+ob->short()+".");
    acquire_follow();
    did_move();
  }
}
