/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>

private string start_location;

int set_start_location(string str)
{
    if ( !stringp(str) )
	return 0;

    if ( load_object(str) )
    {
	start_location = str;
        return 1;
    }

    return 0;
}

string query_start_location()
{
    if ( start_location && load_object(start_location) )
	return start_location;
    if(wizardp(this_user()))
       {
	 return WIZARD_START;
       }
    return START;
}

varargs int move_to_start(object ob) {
    object where;
    
    if (!ob) ob = this_object(); /* move us */
    
    if (start_location && (where = load_object(start_location))
	&& ob->move(where) == MOVE_OK)
	return 1;
    where = load_object(wizardp(this_user()) ? WIZARD_START : START);
    if (where && ob->move(where) == MOVE_OK)
	return 1;

    return (ob->move(VOID_ROOM) == MOVE_OK);
}

