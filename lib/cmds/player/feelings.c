/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;
inherit M_GLOB;

#define HEADER  "--------------------------------<  Feelings  >-------------------------------\n"
#define TRAILER "------------------------<  Found %4d/%4d  (%3d%%)  >------------------------\n"

private void main(string arg)
{
    string * list;
    int count;

    list = SOUL_D->list_emotes();
    count = sizeof(list);

    if ( !arg || arg == "" )
        arg = "*";
    else if ( !has_magic(arg) )
        arg = arg + "*";
    arg = "^"+translate(arg)+"$";
    list = regexp(list, arg);
    if ( !list )
    {
        printf("No feelings matched '%s'.\n", arg);
        return;
    }

    clone_object(MORE_OB)->more_string(
        sprintf(HEADER "%-#79s\n" TRAILER, 
        implode(sort_array(list, 1), "\n"),
        sizeof(list), count, sizeof(list) * 100 / count));
    return;
}
