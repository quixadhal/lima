/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove headers from this file! see /USAGE for more info. */

/* 
 * Object to be inherited by the body object.
 * The purpose of this is to store various histories that are to be associated
 * with the body that don't fit better in other places.  For example 'say 
 * history 
 *
 * Say history size is based upon the CHANNEL_HISTORY_SIZE (see <config.h>)
 * 
 * Written by Tigran 05-08-2000
 */

/* Define this if you want the say history to save in the body's save file.
 * Personally, I think that is going overboard, but someone might want it.
 * This might later be moved to <config.h> -- Tigran
 */
#undef SAY_HISTORY_SAVES

void save_me();

#ifndef SAY_HISTORY_SAVES
nosave
#endif
private string array say_history=({});

void add_say_history(string add) {
  int size;
  
  /* Strip trailing \n's */
  if(add[<1]=='\n')
    add=add[0..<2];
  /* Add the history item to the end of the array */
  say_history+=({add});
  size=sizeof(say_history);
  if(size>CHANNEL_HISTORY_SIZE) { 
    say_history=say_history[(size-CHANNEL_HISTORY_SIZE)..];
  }
}

string array list_say_history() {
  return copy(say_history);
}
