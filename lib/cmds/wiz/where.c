/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;



private void main( mixed * arg )
{
    object*	targets;

    if (!arg[0]) targets = users();
    else targets = arg[0];

    printf("%-13s%-30s%-s\n","Who","File","Room");
    write("---------------------------------------------------------------\n");

    targets = filter_array( targets->query_body(), (: environment :) );

    map_array( targets, (: printf("%-13s%-30s[%-s]\n",
				  capitalize($1->query_real_name()),
				  file_name(environment($1)),
				  environment($1)->short()) :) );

    return;
}
