/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;
inherit M_INPUT;

private nomask void finish()
{
    modal_pop();
    destruct(this_object());
}

private nomask void do_addto(string file, mixed str)
{
    if (str == -1)
      destruct(this_object());
    if ( str == "." || str == "**" )
    {
	finish();
	return;
    }

    /*
    ** Do not use an unguarded here...  this write_file() is perfomed
    ** with the currently outstanding privs
    */
    write_file(file, str + "\n");
}

nomask void addto(string file)
{
    modal_push((: do_addto, file :), (: "]" :));
}
