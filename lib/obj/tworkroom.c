/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** workroom.c
**
** This is used for wizards' temporary workrooms.  This is NOT a template
** for a permanent workroom.  Copy one from an existing wizard or figure
** out how to write one :-)
**
** Deathblade thinks this was written by Beek; certainly commented by DB
*/

#include <mudlib.h>

inherit ROOM;

void set_owner(string str)
{
    set_short(capitalize(str) + "'s Workroom");
    set_long("This place is rather boring.  It is, after all, just temporary :)\n");
    set_light(1);
}
