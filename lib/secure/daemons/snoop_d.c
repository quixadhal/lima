/* Do not remove the headers from this file! see /USAGE for more info. */

#include <playerflags.h>
#define SNOOP_DUMMY "/obj/secure/snoop_dummy"
#define SNOOP_CMD   "/trans/cmds/snoop"
#define UNSNOOP_CMD "/trans/cmds/unsnoop"
#define SNOOPS_CMD  "/trans/cmds/snoops"
mapping cycle_info;
mapping snoops = ([]);
mapping dummy_objects = ([]);
object  array admins;


mapping get_snoop_info()
{
  if(previous_object() != find_object(SNOOPS_CMD))
    {
      return 0;
    }
  // We trust the command, we can pass a reference.
  return snoops;
}

private object get_dummy_object(object snoopee)
{
  object ob = dummy_objects[snoopee];
  if(!ob) 
    {
      ob = new(SNOOP_DUMMY, capitalize(snoopee->query_userid()));
      snoop(ob, snoopee->query_link());
      dummy_objects[snoopee] = ob;
    }
  return ob;
}

// Return 1 if there is a cycle, 0 otherwise.
// This side effects big time, and ends up building a list of admins it
// traversed, so it can tell those people they're now being snooped by
// some new guy...
private int detect_cycle_recursive(object snooper)
{
  object array snoopees;
  int    array cycles;
  if(cycle_info[snooper])
    {
      return 1;
    }
  cycle_info[snooper] = 1;
  if(adminp(snooper)) admins += ({snooper});
  snoopees = snoops[snooper];
  if(!snoopees)
    return 0;
  cycles   = filter(snoopees, (:detect_cycle_recursive:));
  if(sizeof(cycles))
    {
      // clean up
      map_delete(cycle_info, snooper);
      return 1;
    }
  return 0;
}

private int detect_cycle(object snoopee)
{
  cycle_info = ([]);
  admins     = ({});
  return detect_cycle_recursive(snoopee);
}

// Return values:
// 0  == unsnoop OK
// -1 == uh, you weren't snooping him, dumbass.
// -2 == stay the fuck out.

int unsnoop(object snooper, object snoopee)
{
  object array snoopees;
  object target;
  if(origin() != "local"
     && previous_object() != find_object(UNSNOOP_CMD))
    return -2;
  snoopees = snoops[snooper];
  if(!snoopees || member_array(snoopee, snoopees) == -1)
  {
    return -1; // dumbass!
  }
  snoops[snooper] -= ({ snoopee, 0 });
  if(!sizeof(snoops[snooper]))
    {
      map_delete(snoops, snooper);
    }
  // Okay, I know I'm not going to get a cycle, but this function also
  // happens to put a list of admins I'm snooping in the admins array.
  detect_cycle(snoopee);
  target = get_dummy_object(snoopee);
  target->remove_snooper(snooper);
  admins->receive_private_msg(capitalize(snooper->query_userid()) +
                              " is no longer snooping you.\n");
  return 0;
}

// Return values:
// 0  == unsnoop OK
// -1 == uh, you weren't snooping anyone, dumbass.
// -2 == go home
int unsnoop_all(object snooper)
{
  if(previous_object() != find_object(UNSNOOP_CMD))
    return -2;
  if(!snoops[snooper] || !sizeof(snoops[snooper]))
    return -1;
  map(snoops[snooper], (: unsnoop($(snooper), $1) :));
}

// Return values:
// 0  == snoop ok
// -1 == already snooping
// -2 == would make cycle
// -3 == no permissions
// -4 == blow it out your ass.
int snoop(object snooper, object snoopee)
{
  object target;
  if(previous_object() != find_object(SNOOP_CMD))
    return -4;

  // Delete this check if you don't like all the snoopable crap.
  // All the shit gets logged anyway.
  if (!snoopee->test_flag(F_SNOOPABLE) && !adminp(this_user()))
    {
      return -3;
    }

  snoops[snooper] = snoops[snooper] || ({});
  if(member_array(snoopee, snoops[snooper]) != -1)
    {
      return -1;
    }
  
  snoops[snooper] += ({snoopee});
  if(detect_cycle(snoopee))
    {
      snoops[snooper] = snoops[snooper][0..<2];
      return -2;
    }
  target = get_dummy_object(snoopee);
  target->add_snooper(snooper);
  admins->receive_private_msg(capitalize(snooper->query_userid()) +
                              " is now snooping you!\n");
  return 0;
}

void remove()
{
 object array snoopers;

  snoopers = keys(snoops) - ({0});
  snoopers->receive_private_msg("Whoops! SNOOP_D is being dested!\n");
  map(snoopers, (: unsnoop_all :));
  map(values(dummy_objects), (: destruct :));
}

void bye()
{
  object who;
  object dummy;
  object array snoopers;

  who = previous_object();
  if(snoops[who] != 0)
    {
      unsnoop_all(who);
    }
  dummy = dummy_objects[who];
  if(dummy)
    {
      snoopers = dummy->get_snoopers();
      foreach(object snooper in snoopers)
	{
	  unsnoop(snooper, who);
	}
    }
}
