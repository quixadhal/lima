/* Do not remove the headers from this file! see /USAGE for more info. */

// An object that can be opened, closed, locked, and unlocked.
//
// NOTE: this should only be mixed into classes that have
//       M_OPENABLE, too.
// NOTE: due to a bug in the driver(?), you must inherit M_OPENABLE
//       _after_ inheriting M_LOCKABLE.
//

#include <mudlib.h>
#include <hooks.h>

string the_short();
void hook_state(string, function, mixed);
mixed call_hooks(string, int);

private string locked, key_type;
string unlock_msg = "$N $vunlock the $o with $p $o1.\n";
string unlock_fail = "$N $vtry to unlock the $o, but $p $o1 doesn't fit.\n";
string lock_msg = "$N $vlock the $o with $p $o1.\n";
string lock_fail = "$N $vtry to lock the $o, but $p $o1 doesn't fit.\n";
function my_open_hook = (: capitalize(the_short() + " is locked.\n") :);

string query_locked() { return locked; }

void set_locked(string x) {
    locked = x;
    key_type = x;
    hook_state("prevent_open", my_open_hook, locked);
}

void unlock_with(object ob)
{
//:HOOK prevent_unlock
//A yes/no/error hook which can prevent an object from being unlocked.
    mixed ex = call_hooks("prevent_unlock", HOOK_YES_NO_ERROR);
    if (!ex) ex = "You can't seem to unlock it.\n";
    if (stringp(ex)) {
	write(ex);
	return;
    }

    if (ob->key_type() == key_type)
    {
	this_body()->simple_action(unlock_msg, this_object(), ob);
	locked = 0;
	hook_state("prevent_open", my_open_hook, locked);
    }
    else
    {
	this_body()->simple_action(unlock_fail, this_object(), ob);
    }
}

void unlock() {
    object ob = present("key", this_body());
    write("(with " + ob->the_short() + ")\n");
    unlock_with(ob);
}


void lock_with(object ob) {
//:HOOK prevent_lock
//A yes/no/error hook which can prevent an object from being locked.
    mixed ex = call_hooks("prevent_lock", HOOK_YES_NO_ERROR);
    if (!ex) ex = "You can't seem to lock it.\n";
    if (stringp(ex)) {
	write(ex);
	return;
    }
    
    if (ob->key_type() == key_type) {
	this_body()->simple_action(lock_msg, this_object(), ob);
	set_locked(key_type);
    }
    else {
	this_body()->simple_action(lock_fail, this_object(), ob);
    }
}

void lock() {
    object ob = present("key", this_body());
    write("(with " + ob->the_short() + ")\n");
    lock_with(ob);
}

/*********************************************************/
/*    verb interactions                                   */
/**********************************************************/

mixed direct_unlock_obj(object ob) {
    if (!locked)
	return "It isn't locked.\n";
    if (present("key", this_body()))
	return 1;
    return "With what?";
}

mixed direct_lock_obj(object ob) {
    if (locked)
	return "It is already locked.\n";
    if (present("key", this_body()))
	return 1;
    return "With what?";
}

mixed direct_unlock_obj_with_obj(object ob1, object ob2) {
    return 1;
}

mixed direct_lock_obj_with_obj() {
    return 1;
}

