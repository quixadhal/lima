/* Do not remove the headers from this file! see /USAGE for more info. */

// An object that can be opened, closed, locked, and unlocked.
//
// NOTE: this should only be mixed into classes that have
//       M_OPENABLE, too.
// NOTE: due to a bug in the driver(?), you must inherit M_OPENABLE
//       _after_ inheriting M_LOCKABLE.
//

string the_short();


string locked;
string unlock_msg = "$N $vunlock $o with $o1.\n";
string unlock_fail = "$N $vtry to unlock $o, but $o1 doesn't fit.\n";

string query_locked() { return locked; }
void set_locked(string x) { locked = x; }
int prevent_unlock() { return 0; }

int prevent_open()
{
    if (query_locked())
    {
	write(capitalize(the_short())+" is locked.\n");
	return 1;
    }
    return 0;
}

void unlock_with(object ob)
{
    if (this_object()->prevent_unlock())
	return;
    if (ob->key_type() == query_locked())
    {
	this_body()->simple_action(unlock_msg, this_object(), ob);
	set_locked(0);
    }
    else
    {
	this_body()->simple_action(unlock_fail, this_object(), ob);
    }
}

/*********************************************************/
/*    verb interactions                                   */
mixed direct_unlock_obj(object ob)
{

    string contents;
    if (!query_locked())
	return "It is already unlocked.";
    else
    {
	locked = 0;
	return "You unlock the safe. \n";
    }
    return 1;
}

mixed direct_lock_obj(object ob)
{
    if(query_locked())
return "The safe is already locked. \n";
    else
    {
	locked = "1";
	return "You lock the safe. \n";
}
}
