/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** i3chan.c -- Intermud channel administration
**
** 950630, Deathblade: created
*/

#include <mudlib.h>

void std_handler(string str);
varargs void modal_simple(function input_func, int secure);

static nomask void write_i3chan_menu()
{
    write("Administration Tool: Intermud channel administration\n"
	  "\n"
	  "    l        - list owned channels\n"
	  "    a        - add owned channel\n"
	  "    r <name> - remove owned channel\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void list_channels()
{
    mapping chanlist = IMUD_D->query_chanlist();

    chanlist = filter_mapping(chanlist,
			      (: $(chanlist)[$1][0] == mud_name() :));

    write("Owned Channels:");
    if ( sizeof(chanlist) == 0 )
    {
	write("    <none>\n");
	return;
    }

    write(implode(({ "" }) + keys(chanlist),
		  (: $1 + sprintf("    %s/%s\n", $2,
				  $(({"unrestricted",
					  "restricted",
					  "filtered"}))[$(chanlist)[$2][1]])
		   :)));
}

private nomask void rcv_channel_type(string channel_name, string str)
{
    if ( str == "" )
	return;

}

private nomask void rcv_channel_name(string str)
{
    str = lower_case(trim_spaces(str));

    if ( str == "" )
	return;

    if ( IMUD_D->query_chanlist()[str] )
    {
	write("** That channel already exists.\n");
	return;
    }

    IMUD_D->add_channel(str);
}

private nomask void add_channel()
{
    write("New channel name? ");
    modal_simple((: rcv_channel_name :));
}

private nomask void remove_channel(string channel_name)
{
    mapping chanlist = IMUD_D->query_chanlist();

    channel_name = lower_case(channel_name);
    if ( undefinedp(chanlist[channel_name]) )
    {
	write("** That channel does not exist.\n");
	return;
    }

    IMUD_D->remove_channel(channel_name);
}

static nomask void receive_i3chan_input(string str)
{
    string name;

    sscanf(str, "%s %s", str, name);

    switch ( str )
    {
    case "l":
	list_channels();
	break;

    case "a":
	add_channel();
	break;

    case "r":
	remove_channel(name);
	break;

    case "?":
	write_i3chan_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}
