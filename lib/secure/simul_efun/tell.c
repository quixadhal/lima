/* Do not remove the headers from this file! see /USAGE for more info. */

object find_user(string);

varargs void
tell(mixed ob, string what, int msg_type) {
    if (arrayp(ob))
	foreach(object o in ob)
	    o->receive_private_msg(what, msg_type | PRIVATE_MSG);
    else
	ob->receive_private_msg(what, msg_type | PRIVATE_MSG);
}

varargs void
tell_from_inside(mixed ob, string what, int msg_type, mixed exclude) {
    ob->receive_inside_msg(what, exclude, msg_type | INSIDE_MSG);
}

varargs void
tell_from_outside(mixed ob, string what, int msg_type, mixed exclude) {
    ob->receive_outside_msg(what, exclude, msg_type | OUTSIDE_MSG);
}

varargs void
tell_environment(mixed ob, string what, int msg_type, mixed exclude) {
    object env;
    
    if (env = environment(ob))
	env->receive_inside_msg(what, exclude, msg_type | INSIDE_MSG);
}

varargs void
tell_user(string username, string what, int msg_type) {
    object who;
    
    if (who = find_user(username))
	tell(who, what, msg_type);
}

