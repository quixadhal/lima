/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

nomask private void
main(string arg) {
    if (!arg || arg == "") {
        write("No verb supplied.\n");
        return;
    }

    printf("** Enter args to \"%s\". **\n", arg);
    clone_object(EDIT_OB)->edit_text(0, "done_editing", arg);
}

nomask void
done_editing(string verb, string *lines) {
    if (!lines) {
        printf("\"%s\" entry aborted.\n", verb);
        return;
    }

    this_user()->force_me(sprintf("%s %s\n", verb, implode(lines, "\n")));
}
