/* Do not remove the headers from this file! see /USAGE for more info. */

//  timeout_d.c   (timeout daemon)
//  Peregrin@ZorkMUD
//  Feb '94
//
//  This daemon handles auto_saves and idling out of players.
//  By doing it this way, ZorkMUD saves on CPU by greatly reducing
//  the number of call_outs.

// actually, most of the above is bullshit.  -Beek

//  This daemon will initiate auto_saves at SAVE_TIME intervals.
//  For idle_out's, because of the way this daemon works, player's
//  will get up to IDLE_TIME more than their limit, but no more
//  (reading the code may explain why this is so).

#include <timeout.h>
#include <wizlevels.h>
#include <mudlib.h>


#define IDLE_BOOT_MSG  "Sorry, but you have idled too long!\n"
#define IDLED_OUT_MSG  " has been booted for idleness!\n"

int running = 0;
int last_save;
int last_idle;

create() {
  last_save = last_idle = time();
  if (!running) {
    running = 1;
    call_out("idle_out",IDLE_TIME);
    call_out("auto_save",SAVE_TIME);
  }
}

void auto_save() {
  object *users;
  int i;
  if (previous_object() != this_object())  return;
  users = all_users();
  i = sizeof(users);
  while (i--) {
      users[i]->save_me();
	if( !wizardp(users[i]) )
      tell_object(users[i],"(Autosave)\n");
      users[i]->save_autoload();
  }
  call_out("auto_save",SAVE_TIME);
  last_save = time();
}

void idle_out() {
  object *users;
  int i, level, idle_time;
  if (previous_object() != this_object())  return;
  users = all_users();
  i = sizeof(users);
  while (i--) {
    if (GROUP_D->adminp(users[i]))  continue;
    level = users[i]->query_level();
    if (level < APPRENTICE)
      idle_time = PLAYER_I;
    else if (level < WIZARD)
      idle_time = APPRENTICE_I;
    else if (level < SENIOR)
      idle_time = WIZARD_I;
    else if (level < ELDER)
      idle_time = SENIOR_I;
    else if (level < ARCH)
      idle_time = ELDER_I;
    else if (level < ADMIN)
      idle_time = ARCH_I;

    if (idle_time && query_idle(users[i]) > idle_time) {
      tell_object(users[i],IDLE_BOOT_MSG);
      tell_room(environment(users[i]),users[i]->query_name()+IDLED_OUT_MSG,({users[i]}));

      /* ### added this catch.. this is a hack, but for now... */
      catch(users[i]->force_me("quit"));
    }
  }
  call_out("idle_out",IDLE_TIME);
}

mixed *timeout_info() {
  return ({ last_save, last_idle, all_users() });
}
