/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** look.c
**
*/

#include <verbs.h>

//### peer should be a synonym for some, but not all, of these rules.
//### E.g. not "" or it will interfere with the emote


inherit NVERB_OB;
inherit M_GRAMMAR;

void create() {
    clear_flag(NEED_TO_BE_ALIVE);

    add_rules( ({ "", "STR","at OBS:v", "for OBS:v", "STR OBJ:v",
		      "at OBS:v with OBJ", "WRD OBJ" }) );

    add_rules( ({ "OBS:v", "OBS:v with OBJ" }), ({ "examine" }) );
}

mixed can_look_wrd_obj(string wrd, object ob)
{
    if (wrd == "through" ) return default_checks();
    return 0;
}

mixed can_look_str(string str) {
#if 0 // deprecated
    mixed value = environment(this_body())->query_exit_value(str);
    if (value && value[0] != '#')
	return default_checks();
#endif
    return "That doesn't seem to be possible.\n";
}

void do_look() {
    this_body()->force_look(1);
#ifdef USE_STATUS_LINE
    if( this_body()->has_status_line()) this_body()->update_status_line();
#endif
}

void do_look_at_obj(object ob, string name) {
    string str;

    if (!(str = ob->get_item_desc(name)))
	str = ob->long();

    if (sizeof(str) && str[<1] != '\n') str += "\n";
    write(str);
}

void do_look_at_obs(array info, string name) {
    handle_obs(info, (: do_look_at_obj :), name);
}

void do_look_str_obj(string prep, object ob) {
    write(ob->look_in(prep)+"\n");
}

void do_look_obj(object ob, string name) {
    do_look_at_obj(ob, name);
}

void do_look_obs(array info, string name) {
    handle_obs(info, (: do_look_at_obj :), name);
}

void do_look_at_obj_with_obj(object o1, object o2) {
    // o2->indirect_look_at_obj_with_obj() has already indicated this is ok.
    o2->use("look", o1);
}

void do_look_at_obs_with_obj(array info, object o2) {
    handle_obs(info, (: do_look_at_obj_with_obj :), o2);
}

void do_look_obj_with_obj(object o1, object o2) {
    do_look_at_obj_with_obj(o1, o2);
}

void do_look_obs_with_obj(array info, object o2) {
    handle_obs(info, (: do_look_at_obj_with_obj :), o2);
}

void do_look_for_obj(object ob) {
    object env = environment(ob);
    string relation;

    if (!env) {
	// the room
	write("You're on it!\n");
	return;
    }
    if (ob == this_body()) {
	write("Trying to find yourself?\n");
	return;
    }
    if (environment(this_body()) == env) {
	//### we need a way to conjugate this
	this_body()->my_action("The $o0 is right here!", ob);
	return;
    }
    if(env->is_living())
    {
	this_body()->my_action("$O $vis carrying it.", env);
	return;
    } else {
	relation = env->query_prep(ob);

	// printf("%O %O\n", ob, env);
	this_body()->my_action("The $o0 is " + relation + " the $o1.",
	  ob, env);
    }
}

string look_for_phrase(object ob) {
    object env = environment(ob);

    if (env == environment(this_body()))
	return "on the ground";
    if(env->is_living())
	return "carried by " + env->the_short();
    return env->query_prep(ob) + " " + env->the_short();
}

void do_look_for_obs(array info) {
    mixed ua;
    int i, n;
    string res;

    /* ignore errors */
    info = filter(info, (: objectp :));
    ua = unique_array(info, (: look_for_phrase :));

    n = sizeof(ua);
    res = "There " + (sizeof(ua[0]) > 1 ? "are " : "is ");
    for (i = 0; i < n; i++) {
	if (i != 0) {
	    if (i == n - 1)
		res += " and ";
	    else
		res += ", ";
	}
	res += number_word(sizeof(ua[i])) + " " + look_for_phrase(ua[i][0]);
    }
    write(res + ".\n");
//### why?
#if 0
    if (environment(this_body()) == env) {
	this_body()->my_action("$O is right here!", ob);
    } else {
	relation = env->query_prep(ob);

	this_body()->my_action("The $O is " + relation + " the $O.",
	  ob, env);
    }
#endif
}

mixed
do_look_wrd_obj(string wrd, object ob)
{
    ob->look(wrd);
}

    /*
    ** "examine OBJ" -> "look OBJ"
    ** "examine OBS" -> "look OBS"
    ** "examine OBS with OBJ" -> "look OBS with OBJ"
    ** "examine OBJ with OBJ" -> "look OBJ with OBJ"
    ** "gaze at OBJ" -> "look at OBJ"
    ** "gaze at OBS" -> "look at OBS"
    ** "gaze at OBS with OBJ" -> "look at OBS with OBJ"
    ** "gaze at OBJ with OBJ" -> "look at OBJ with OBJ"
    ** "gaze STR" -> "look STR"
    ** "gaze up" -> "look up"
    ** "gaze down" -> "look down"
    ** "stare 1" -> "look 1"
    ** "stare at OBJ" -> "look at OBJ"
    ** "stare at OBS" -> "look at OBS"
    ** "stare at OBS with OBJ" -> "look at OBS with OBJ"
    ** "stare at OBJ with OBJ" -> "look at OBJ with OBJ"
    ** "stare STR" -> "look STR"
    ** "stare up" -> "look up"
    ** "stare down" -> "look down"
    */
