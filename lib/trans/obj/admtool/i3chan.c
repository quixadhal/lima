/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** i3chan.c -- Intermud channel administration
**
** 950630, Deathblade: created
*/

#include <mudlib.h>

void std_handler(string str);
void do_one_arg(string arg_prompt, function fp, string arg);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_I3CHAN	"(AdmTool:i3chan) [larmq?] > "


private nomask void write_i3chan_menu()
{
    write("Administration Tool: Intermud channel administration\n"
	  "\n"
	  "    l        - list owned channels\n"
	  "    a [name] - add owned channel\n"
	  "    r [name] - remove owned channel\n"
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

private nomask void rcv_add_channel(string channel_name)
{
    channel_name = lower_case(trim_spaces(channel_name));
    if ( channel_name == "" )
	return;

    if ( IMUD_D->query_chanlist()[channel_name] )
    {
	write("** That channel already exists.\n");
	return;
    }

//### need to get a channel type...
    IMUD_D->add_channel(channel_name);
}

private nomask void rcv_remove_channel(string channel_name)
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

private nomask void receive_i3chan_input(string str)
{
    string arg;

    sscanf(str, "%s %s", str, arg);

    switch ( str )
    {
    case "l":
	list_channels();
	break;

    case "a":
	do_one_arg("New channel name? ", (: rcv_add_channel :), arg);
	break;

    case "r":
	do_one_arg("Channel name to remove? ", (: rcv_remove_channel :), arg);
	break;

    case "?":
	write_i3chan_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_i3chan_menu()
{
    if ( !check_privilege("Mudlib:daemons") )
    {
	write("Sorry... Mudlib:daemons priv-holders only.\n");
	return;
    }
    modal_func((: receive_i3chan_input :), PROMPT_I3CHAN);
    write_i3chan_menu();
}
