/* Do not remove the headers from this file! see /USAGE for more info. */
#define TOO_DAMN_LONG 15
#include <mudlib.h>

inherit CMD;
inherit M_GLOB;

#define HEADER  "--------------------------------<  Feelings  >-------------------------------\n"
#define TRAILER "------------------------<  Found %4d/%4d  (%3d%%)  >------------------------\n"

private void main(string arg)
{
    string * list;
    int count;
    string * too_damn_long;
    string * not_too_long;
    string   str;

    list = sort_array(SOUL_D->list_emotes(),1);
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
    too_damn_long = filter(list, (: sizeof($1) >= TOO_DAMN_LONG :));
    not_too_long = filter(list, (: sizeof($1) < TOO_DAMN_LONG :));

/*    clone_object(MORE_OB)->more_string(
        sprintf(HEADER "%-#79s%s\n" TRAILER, 
        implode(not_too_long, "\n"),
	(sizeof(too_damn_long) ? ("Souls that are too damn long:\n" +
		implode(too_damn_long,"\n")) : ""),
        sizeof(list), count, sizeof(list) * 100 / count));
*/
    str = HEADER + sprintf("%-#79s\n",implode(not_too_long,"\n")) +
      (sizeof(too_damn_long) ? ("\nSouls that are too damn long:\n" +
				implode(too_damn_long,"\n")+"\n") : "") +
      sprintf(TRAILER, sizeof(list), count, sizeof(list) * 100 / count);
    this_user()->more(str);
    return;
}
