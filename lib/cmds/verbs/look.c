/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** look.c
**
*/

//### peer should be a synonym for some, but not all, of these rules.
//### E.g. not "" or it will interfere with the emote

#include <mudlib.h>

inherit VERB_OB;
inherit M_GRAMMAR;

int need_to_be_alive() {
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
    environment(this_body())->do_looking(1);
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
    string str;

    handle_obs(info, (: do_look_at_obj :), name);
}

// There was name support here, but it was done wrong, as according to 
// container.c the first arg of look_in() is a prep, not a name.  Anyway,
// if you're looking for what happened to that code, that's why I removed it.
// -Beek
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
	this_body()->my_action("The $o0 is right here!\n", ob);
    } else {
	relation = env->query_prep(ob);

	printf("%O %O\n", ob, env);
	this_body()->my_action("The $o0 is " + relation + " the $o1.\n",
			       ob, env);
    }
}

string look_for_phrase(object ob) {
    object env = environment(ob);

    if (env == environment(this_body()))
	return "on the ground";
    return env->query_prep(ob) + " " + env->the_short();
}

void do_look_for_obs(array info) {
    mixed ua;
    array k;
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
#if 0
    if (environment(this_body()) == env) {
	this_body()->my_action("$O is right here!\n", ob);
    } else {
	relation = env->query_prep(ob);
	
	this_body()->my_action("The $O is " + relation + " the $O.\n",
			       ob, env);
    }
#endif
}

array query_verb_info()
{
    return ({ 
         ({ "OBS:v", "OBS:v with OBJ" }), ({ "examine" }),
	 ({ "", "at OBS:v", "for OBS:v", "STR OBJ:v",
		"at OBS:v with OBJ" })
    });
    
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
}


#ifdef OLD_CODE

#include <mudlib.h>

inherit M_PARSING;

static private string * mypreps;

create()
{
    mypreps = ({"in","behind","under","with", "on", "down", "around" });
}

int look( int rule, mixed stack, mixed input )
{
    mapping ex;
    object* obs;

    switch( rule )
    {
    case 1:
	environment( this_body() )->do_looking();
	return 1;
    case 2:
	if( !stringp( stack[2] ) )
	    write("Right here!\n");
	else
	    if( find_player( stack[2] ) )
		printf("Search around a bit...\n");
	    else
		write("I'm sure that's around here somewhere...\n");
	return 1;
    case 3:
	if( sizeof( stack ) == 5 && stack[1] == "at" &&
	   (objectp(stack[2]) || pointerp(stack[2])) && stringp( stack[3] )
	   && objectp( stack[4] ) )
	{
	    mixed p;

	    p = condense_phrase( stack[2..4] );
	    if( !pointerp( p ) )
		return p;

	    if( sizeof( p ) != 1 )
		return write( "Try just looking at one thing "
				   "at a time.\n" ), 0;

	    return write( stack[2]-> long() ), 1;

	}
	if( sizeof( stack ) != 3 )
	    return 0;
	if( !objectp( stack[2] ) )
	    return 0;
	if( member_array(stack[1], mypreps) == -1)
	    return 0;

	return write( stack[2]->look_in(stack[1]) ), 1;
    case 4:
	stack = ({""}) + stack;

    case 5:
	if( stack[2] == environment( this_body() ) )
	    return write(
			 environment(this_body())->show_item_desc(input[8..])),1;
	return write( stack[2]->long() ), 1;
    case 6:
	write( "Try just looking at one thing at a time.\n" );
	return 0;
    case 11:
	stack = ({""})+stack;
    case 7:
	return write( stack[4]->use("look", stack[2] ) ), 1;
    case 10:
	ex = environment(this_body())->get_exits();
	if ( member_array(ex, stack[1]) )
	{
	    write(find_object(ex[stack[1]])->
		      remote_look(environment(this_body())));
	    return 1;
	}

	write( "That doesn't seem to be possible.\n" );
	return 1;
    }
}

#endif /* OLD_CODE */
