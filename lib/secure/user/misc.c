/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * misc.c -- Miscellaneous functions for the user
 *
 * 042800 -- Tigran Created from the old /std/body/wizfuncs.c
 */

void save_me();
varargs void receive_private_msg(string);

private int didlog_time;		// The last time the didlog was read
private int didlog_off = 0;		// Does the user want to read the 
                                        // didlog at all?

/* 
 * Didlog handling 
 */

//:FUNCTION set_didlog_time
//Set the last time that the didlog was read for the user.
protected void set_didlog_time(int t) {
    didlog_time = t;
    save_me();
}
 
//:FUNCTION query_didlog_time
//Return the time that the user last read the didlog.
int query_didlog_time() {
  if(!didlog_time)
    didlog_time=time();
  return didlog_time;
}

//:FUNCTION deliver_didlog_message
//Deliver a didlog message while someone is still logged in
void deliver_didlog_message(string str)
{
  if ( !didlog_off )
    {
      receive_private_msg("*** " + str + "\n");
      set_didlog_time(time());
    }
}

//:FUNCTION set_didlog_off
//If nonzero, this turns off the automatic posting of the didlog for the user
void set_didlog_off(int x) {
  didlog_off = x;
}

//:FUNCTION query_didlog_off
//Returns 1 if the user does not receive the automatic posting of the didlog
int query_didlog_off() {
  return didlog_off!=0;
}

//:FUNCTION display_didlog
//Displays didlog activity since the last time the user has checked it.
//Primarily used in the login process.
void display_didlog() {
  if ( wizardp(this_object()) ) {
    DID_D->dump_did_info(query_didlog_time(),
			 ({ "",
			      "Changes since you last logged in",
			      "********************************",
			      "" }),
			 0);
    set_didlog_time(time());
    return;
  }
}
