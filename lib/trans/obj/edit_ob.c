/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** edit_ob.c
**
** Based on a text-entry facility by Rust.  
** Transformed and rewritten by Deathblade on 8/23/94 to use the
** new input system facilities.
**
** Usage:
**     clone_object(EDIT_OB)->edit_file(fname, func, ctx)
** or  clone_object(EDIT_OB)->edit_text(text, func, ctx)
**
** The calling object will then have "func" called on it when
** the text entry is complete.  The arguments are as follows:
**
**     void my_func(mixed ctx, string fname|string *text)
**
** The second parameter will contain the file name or the lines
** of text.  If the editing was aborted, the second param will
** be zero.
*/

#include <mudlib.h>
#include <playerflags.h>

/*
** Inherit from DAEMON so that we clean up properly and we have some
** privileges (we're in the call chain for some cases where the callback
** needs privileged access).
*/
inherit DAEMON;
inherit M_INPUT;

#define HEADER \
"Begin typing your text. ~q to abort, . or ** to end, ~h for help.\n"+\
"-----------------------------------------------------------------\n"

private string* buf;

private string client_fname;
private object client_ob;
private string client_func;
private mixed client_ctx;



private string tmp_fname()
{
    return "/tmp/edit_ob." + this_user()->query_real_name();
}

private string * read_strings(string fname, int messages)
{
    int size;

    size = file_size(fname);
    if ( messages )
    {
	if ( size == -2 )
	    write("File is a directory. Ignoring.\n");
	else if ( size == -1 )
	    write("File not found.\n");
	else if ( size == 0 )
	    write("File is empty.\n");
	else
	    write("Ok.\n");
    }
    if ( size <= 0 )
	return ({ });

    return explode(read_file(fname), "\n");
}

varargs private string build_string(int flag)
{
    if ( sizeof(buf) == 0 )
      return flag ? "**No text!\n" : "";
    return implode(buf, "\n") + "\n";
}

private void end_edit(int aborted)
{
    mixed arg2;

    if ( client_fname )
    {
	rm(client_fname);

	if ( !aborted )
	{
            write_file(client_fname, build_string(1));
	    arg2 = client_fname;
	}
    }
    else if ( !aborted )
	arg2 = buf;

    /*
    ** Clear up this information before dispatching the callback.  The
    ** callback may want to push new modal handlers or something.
    */
    this_body()->clear_flag(F_IN_EDIT);
    modal_pop();

    if(functionp(client_func))
	evaluate(client_func, client_ctx, arg2);
    else
    	call_other(client_ob, client_func, client_ctx, arg2);

    destruct(this_object());
}

private nomask void end_ed()
{
    string tmp_file;

    tmp_file = tmp_fname();
    buf = read_strings(tmp_file, 0);
    rm(tmp_file);

    write("Continue editing file.\n");
}

private void handle_escape(string str)
{
    string tmp;
    string tmp_file;

    switch ( str[1] )
    {
    case 'q':
	write("Edit aborted.\n");
	end_edit(1);
	return;
    case 'r':
	if(wizardp(this_user()))
	    buf += read_strings(evaluate_path(str[3..]), 1);
	return;
    case 'w':
	if(wizardp(this_user()))
	{
	    tmp = build_string();
	    if(!write_file(evaluate_path(str[3..]),tmp))
		write("Unable to write to file.\n");
	    else
		write("Ok.\n");
	}
	return;
    case 'h':
	write(
	  "Help for editor:\n. or **\t\texit editor\n~q\t\tabort edit.\n~h\t\tthis help.\n"+
	  "~e\t\tenter line by line editor.\n~p\t\tdisplay edit buffer.\n");
	if(wizardp(this_user()))
	    write("~w <file>\twrite buffer to specified file.\n~r <file>\tread file into buffer.\n");
	write("\n\n");
	return;
    case 'p':
        write(build_string() + "\n");
	return;
    case 'e':
	tmp_file = tmp_fname();
        write_file(tmp_file, build_string());
	clone_object(ED_SESSION)->begin_editing(tmp_file, 0, (: end_ed :));
	return;
    }
}

private nomask void parse_edit(mixed str)
{
    if(str == -1)
      destruct(this_object());
    if ( str[0] == '~' )
    {
	handle_escape(str);
	return;
    }

    if ( str == "." || str == "**" )
    {
	end_edit(0);
	return;
    }

    buf += ({ str });
}

private nomask string query_prompt()
{
    return "";    // could be "]" to look like some others
}

private void begin_edit(string *text, string func, mixed ctx)
{
    if ( this_body()->test_flag(F_IN_EDIT) )
	write("Warning! You are already marked as editing.\n");
    this_body()->set_flag(F_IN_EDIT);

    buf = text ? text : ({ });
    client_ob = previous_object();
    client_func = func;
    client_ctx = ctx;

    write(HEADER + build_string());
    modal_push((: parse_edit :), (: query_prompt :));
}

nomask void edit_file(string file, string func, mixed ctx)
{
    string *text;

    client_fname = file;
    text = read_strings(file, 0);

    begin_edit(text, func, ctx);
}

nomask void edit_text(mixed text, string func, mixed ctx)
{
    if ( stringp(text) )
	text = explode(text, "\n");
    begin_edit(text, func, ctx);
}
