/* Do not remove the headers from this file! see /USAGE for more info. */
// This looks like addto??  Read addto's header.

#include <mudlib.h>
#include <edit.h>

inherit CMD;

private nomask void done_editing(string verb, string *lines)
{
    if (!lines) {
        printf("\"%s\" entry aborted.\n", verb);
        return;
    }

    this_user()->force_me(sprintf("%s %s\n", verb, implode(lines, "\n")));
}

private nomask void main(string arg)
{
    if (!arg || arg == "") {
        write("No verb supplied.\n");
        return;
    }

    printf("** Enter args to \"%s\". **\n", arg);
    new(EDIT_OB, EDIT_TEXT, 0, (: done_editing, arg :));
}
