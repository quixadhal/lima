/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** verb_d.c
**
** Manages all of the verbs in the system.  There only needs to be
** a single copy of this object, so we use a daemon.
**
** NOTE: it is possible to add all of the parse rules here rather
** than in the individual verb files.  This allows a person to
** see all of the verbs as a whole.  Doing it that way, though,
** is not very encapsulated.  If each verb object does it, then
** that object can define the rules AND the responses for each
** rule.  Breaking up those two concepts means that this file
** would have to know the rules that the verb object can understand.
*/

#include <mudlib.h>
#include <commands.h>

/*
** reload_verb()
**
** Load or reload the given verb file
*/
private void reload_verb(string file)
{
    object ob;

    if ( ob = find_object(CMD_DIR_VERBS "/" + file) )
    {
	ob->remove();
	if ( ob )
	    destruct(ob);
    }

    call_other(CMD_DIR_VERBS "/" + file, "???");
}

/*
** create()
**
** When this daemon is loaded, get each of the verb objects loaded.
** Each verb object will define its rules.
*/
void create()
{
    string * files;

    files = get_dir(CMD_DIR_VERBS "/*.c");
    map_array(files, (: reload_verb :));
}
