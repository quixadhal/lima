/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove headers from this file! see /USAGE for more info. */

/* 
 * Object to be inherited by the user object.
 * The purpose of this is to store various histories that are to be associated
 * with the user that don't fit better in other places.  For example tell 
 * history 
 *
 * Tell history size is based upon the CHANNEL_HISTORY_SIZE (see <config.h>)
 * 
 * Written by Tigran 06-16-2000
 */

/* Define this if you want the tell history to save in the user's save file.
 * Personally, I think that is going overboard, but someone might want it.
 * This might later be moved to <config.h> -- Tigran
 */
#undef TELL_HISTORY_SAVES

#ifndef TELL_HISTORY_SAVES
nosave
#endif
private string array tell_history=({});

private nosave string reply;

void add_tell_history(string add) {
  int size;
  
  /* Strip trailing \n's */
  if(add[<1]=='\n')
    add=add[0..<2];
  /* Add the history item to the end of the array */
  tell_history+=({add});
  size=sizeof(tell_history);
  if(size>CHANNEL_HISTORY_SIZE) { 
    tell_history=tell_history[(size-CHANNEL_HISTORY_SIZE)..];
  }
}

string array list_tell_history() {
  return copy(tell_history);
}


//:FUNCTION set_reply
//set_reply(s) sets the person to whom 'reply' goes to.
void set_reply(string o)
{
    reply = o;
}

//:FUNCTION query_reply
//query the person to whom reply goes to
string query_reply()
{
    return reply;
}
