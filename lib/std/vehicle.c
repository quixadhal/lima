/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** vehicle.c
**
** An "outside" object that can be driven around.
**
** 951123, Deathblade: created
*/

inherit OBJ;
inherit M_OUTSIDE;
inherit M_SMARTMOVE;

int is_vehicle()
{
    return 1;
}

//### this isn't right, but I didn't want to redesign _too_ much just yet
string *get_player_message(string message, mixed arg) {
    string mess;

    if ( message == "leave" )
	mess = "$N $vleave $o.\n";
    else if ( message == "enter" )
	mess = "$N $venter.\n";

    return M_MESSAGES->action( ({ this_object() }), mess, arg);
}
