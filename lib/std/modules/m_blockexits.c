/* Do not remove the headers from this file! see /USAGE for more info. */

private mixed handle_blocks(string str);
private void we_moved();

string array current_blocks = ({});
function hook = (: handle_blocks :);
function move_hook = (: we_moved :);
object env;
mixed block_action;

void add_hook(string, mixed);
void remove_hook(string, mixed);

private void we_moved() {
    if (env) {
	foreach (string dir in current_blocks)
	    env->remove_hook("block_" + dir, hook);
    }
    else
      {
	foreach (string dir in current_blocks)
	    remove_hook("block_" + dir, hook);
      }
    env = environment();
    if (env) {
	foreach (string dir in current_blocks)
	    env->add_hook("block_" + dir, hook);
    }
    else
      {
	foreach (string dir in current_blocks)
	    add_hook("block_" + dir, hook);
      }
}

//:FUNCTION add_block
//Adds the exits specified to the list of exits this object is blocking
//'all' can be used to block all exits
void add_block(string array exits...) {
    current_blocks += exits;
    if (env)
	foreach (string dir in exits)
	    env->add_hook("block_" + dir, hook);
    else
	foreach (string dir in exits)
	    add_hook("block_" + dir, hook);
    if (sizeof(current_blocks))
	add_hook("move", move_hook);
}

//:FUNCTION remove_block
//Removes the exits specified to the list of exits this object is blocking
void remove_block(string array exits...) {
    current_blocks -= exits;
    if (env)
	foreach (string dir in exits)
	    env->remove_hook("block_" + dir, hook);
    else
	foreach (string dir in exits)
	    remove_hook("block_" + dir, hook);
    if (!sizeof(current_blocks))
	remove_hook("move", move_hook);
}

//:FUNCTION set_block_action
//If given a string, that 'targetted_action' will be done when someone
//tries to move a direction we are blocking, unless it starts with a
//'#', in which case it is just written to the user.  If it is a function,
//then that function will be called with the direction and the person
//as args, and the move will be allowed only if the function returns 1.
void set_block_action(mixed ba) {
    block_action = ba;
}

varargs void targetted_action();

mixed handle_blocks(string dir) {
    // sanity check
    if (env != environment(this_object())) {
	we_moved();
	return 0; // don't block, we weren't even in the room we thought
	          // we were
    }
    if (stringp(block_action)) {
	if (block_action[0] == '#')
	    write(block_action[1..]);
	else
	    targetted_action(block_action, this_body(), dir, this_object());
	return 1;
    }
    if(!functionp(block_action))
	error("Blocked exit, but set_block_action() not called.\n");
    
    return !evaluate(block_action, dir, this_body());
}
