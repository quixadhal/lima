/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** wiz_position.c -- supply positions for wizardly types
**
** Positions are simple strings assigned by admins to a wizard to describe
** their relation/job/position/whatever to the mud.
**
** 951213, Deathblade: created
*/

private string wiz_position;

nomask string query_wiz_position()
{
    return wiz_position;
}

nomask void set_wiz_position(string new_position)
{
    if ( !check_previous_privilege(1) )
	error("Illegal attempt to set a position.\n");

    wiz_position = new_position;
}
