/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** ed_session.c
**
** Represents an editing session.  This is used in conjunction with the
** input system to allow nested editing/more/help/etc.
**
** 23-Jan-95. Deathblade. Created.
*/

#include <mudlib.h>
#include <playerflags.h>

inherit DAEMON;		/* for cleanup and privs */

inherit M_INPUT;

static private string	real_name;	/* ### HACK */
static private function	end_func;


private nomask void receive_ed_input(mixed s)
{
    if(s == -1)
      destruct(this_object());
  
    write(ed_cmd(s));
    if ( query_ed_mode() == -1 )
    {
	modal_pop();
	this_body()->clear_flag(F_IN_EDIT);	/* ### does not nest! */

	if ( end_func )
	    evaluate(end_func);

	destruct(this_object());
    }
}

private nomask string query_prompt()
{
    int line;

    line = query_ed_mode();
    if ( line == -2 )
	return "";

    if ( line )
	return "";

    return ":";
}

/* ### until we decide how to rewrite the master ob (securely) */
nomask string query_real_name()
{
    return real_name;
}

varargs nomask void begin_editing(string fname,
				  int restricted,
				  function f)
{
    modal_push((: receive_ed_input :), (: query_prompt :));
    this_body()->set_flag(F_IN_EDIT);		/* ### does not nest! */

    real_name	= this_user()->query_userid();
    end_func	= f;

    ed_start(fname, restricted);
    printf("Editing: /%s", ed_cmd("f"));
    if(!is_file(fname))
      printf("[New file]\n");
}
