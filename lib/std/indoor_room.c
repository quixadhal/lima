/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BASE_ROOM;

int is_indoors()
{
    return 1;
}

void mudlib_setup()
{
    ::mudlib_setup();
    add_id_no_plural("ground");
    add_id("room");
}
