/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wolf.c
**
** 951013: Initial concept by Rackain@Red Dragon.  Following behavior
**         added by Deathblade@Lima Bean.
** 951013: Beek rewrote parts of it to better and use improved mudlib
**         support.
** 980510  Tigran rewrote to use FOLLOWING_MONSTER
*/
#include <mudlib.h>

#define REGEXP_SSCANF_BUG

inherit FOLLOWING_MONSTER;
inherit M_TRIGGERS;

private string array masters = ({ "tigran" });

void do_follow(string name) {
  object ob;
  
  name = lower_case(name);
  if (!(ob = present(name, environment())) || !ob->is_living()) {
    respond("growl angrily");
    return;
  }
  set_follow(ob);
}

void do_command(string who, string str) {
  if (str[<1] == '.') str = str[0..<2];
  if (member_array(who, masters) == -1) {
    respond("growl angrily");
    return;
  }
  if (str[0..6] == "follow ") 
    do_follow(str[7..]);
  else
    respond(str);
}

void setup()
{
  int *handle;
  
  set_name("Wolf");
  set_gender(1);
  set_in_room_desc("A White Timber Wolf");
  set_adj("White Timber", "timber", "white");
  set_id("Wolf", "wolf");
  set_unique(1);
  set_long("A White Timber Wolf");
  add_pattern("%s says: Timber, %s", (: do_command :));
  //  add_pattern("%([^ ]*) enters", (: "emote glances at " + $1 + " for a moment." :));
  add_pattern("%s looks at you", (: "look " + $1 :));
  set_follow_search(masters);
}

