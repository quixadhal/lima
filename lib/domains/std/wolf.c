/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wolf.c
**
** 951013: Initial concept by Rackain@Red Dragon.  Following behavior
**         added by Deathblade@Lima Bean.
** 951013: Beek rewrote parts of it to better and use improved mudlib
**         support.
*/
#include <mudlib.h>

#define REGEXP_SSCANF_BUG

inherit LIVING;
inherit M_ACTIONS;
inherit M_TRIGGERS;

void follow_callback(string);

private static object target, target_where;
private static function f_follow = (: follow_callback :);
private string array masters = ({ "Beek", "Rackain" });

void i_moved() {
    if (target_where)
	target_where->remove_hook("person_left", f_follow);
    
    target_where = environment();
    
    if (target_where)
	target_where->add_hook("person_left", f_follow);
}

void set_target(object new_target)
{
    if (target = new_target)
	i_moved(); // update trigger
    else if (target_where) {
	target_where->remove_hook("person_left", f_follow);
	target_where = 0;
    }
}

void follow_callback(string dir) {
    if (this_body() != target) return;
    respond("go " + dir);
}

object query_target()
{
    return target;
}

void do_follow(string name) {
    object ob;
    
    name = lower_case(name);
    if (!(ob = present(name, environment())) || !ob->is_living()) {
	respond("growl angrily");
	return;
    }
    set_target(ob);
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

void create()
{
    int *handle;
    
    ::create();
    set_name("Wolf");
    set_gender(1);
    set_in_room_desc("A White Timber Wolf");
    set_adj("White Timber", "timber", "white");
    set_id("Wolf", "wolf");
    set_unique(1);
    
    set_long("A White Timber Wolf");

    add_pattern("%s says: Timber, %s", (: do_command :));
//### regexp sscanf bug
//### There should be hooks for these so we don't have to catch_tell it.
//### say as well.
#ifdef REGEXP_SSCANF_BUG
    add_pattern("%([^ ]*) enters", (: "emote glances at " + $1 + " for a moment." :));
#else
    add_pattern("%s enters", (: "emote glances at " + $1 + " for a moment." :));
#endif
    add_pattern("%s looks at you", (: "look " + $1 :));

    add_hook("move", (: i_moved :));
}

