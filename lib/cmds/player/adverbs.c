/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_GLOB;

#define HEADER  "--------------------------------<  Adverbs  >-------------------------------\n"
#define TRAILER "------------------------<  Found %4d/%4d  (%3d%%)  >------------------------\n"

int main(string arg)
{
    string * list;
    int count;

    list = SOUL_D->get_adverbs();
    count = sizeof(list);

    if ( !arg || arg == "" )
        arg = "*";
    else if ( !has_magic(arg) )
        arg = arg + "*";
    arg = "^"+translate(arg)+"$";
    list = regexp(list, arg);
    if ( !list )
    {
        printf("No adverbs matched '%s'.\n", arg);
        return 1;
    }

    clone_object(MORE_OB)->more_string(
        sprintf(HEADER "%-#79s\n" TRAILER, 
        implode(sort_array(list, 1), "\n"),
        sizeof(list), count, sizeof(list) * 100 / count));
    return 1;
}
