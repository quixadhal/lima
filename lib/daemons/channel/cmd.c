/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** channel/cmd.c
**
** Part of the CHANNEL_D to handle channel command processing.
*/

#include <classes.h>
#include <channel.h>

inherit CLASS_CHANNEL_INFO;

void create_channel(string channel_name);
class channel_info query_channel_info(string channel_name);
string user_channel_name(string channel_name);
void test_for_purge(string channel_name);

void set_permanent(string channel_name, int is_perm);
void set_flags(string channel_name, int flags);

void deliver_emote(string channel_name,
  string sender_name,
  string message);
void deliver_tell(string channel_name,
  string sender_name,
  string message);
void deliver_soul(string channel_name, mixed * soul);

string make_name_list(mixed * list);

int cmd_moderation(string channel_name, string arg);
void moderation_signoff(string channel_name);
void print_mod_info(string channel_name);


/*
** cmd_channel()
**
** Standard channel processing command for player input.  Most channel-
** oriented systems will use this to get standardized channel manipulation.
**
** channel_type is:
**	0 normal
**	1 intermud
*/
varargs nomask void cmd_channel(string channel_name, string arg,
  int channel_type)
{
    class channel_info ci = query_channel_info(channel_name);
    object user=this_user();
    int listening;
    string user_channel_name;
    string sender_name;

    listening = member_array(channel_name, user->query_channel_list()) != -1;
    user_channel_name = user_channel_name(channel_name);

    if ( !arg || arg == "" )
    {
	if ( listening )
	{
	    printf("You are presently listening to '%s'.\n", user_channel_name);
	    print_mod_info(channel_name);
	}
	else
	{
	    printf("You are not listening to '%s'.\n", user_channel_name);
	}

	return;
    }

    if ( arg[0..3] == "/new" )
    {
	string * options = explode(arg[4..], " ");

	if ( ci )
	{
	    if ( sizeof(options) )
		printf("'%s' already exists; modifying options...\n",
		  user_channel_name);
	    else
		printf("'%s' already exists.\n", user_channel_name);
	}
	else
	{
	    create_channel(channel_name);
	    printf("'%s' has been created.\n", user_channel_name);
	}

	ci = query_channel_info(channel_name);

	foreach ( string option in options )
	{
	    switch ( option )
	    {
	    case "admin":
		if ( !adminp(user) )
		{
		    printf("Only admins can create admin channels.\n");
		    return;
		}
		set_flags(channel_name, CHANNEL_ADMIN_ONLY);
		printf("  --> only admins may tune in\n");
		break;

	    case "wiz":
	    case "wizard":
		/* ### need better security? */
		if ( (ci->flags & CHANNEL_ADMIN_ONLY) && !adminp(user) )
		{
		    printf("Only admins can turn off admin-only.\n");
		    return;
		}
		else if ( !wizardp(user) )
		{
		    printf("Only wizards can create wizard channels.\n");
		    return;
		}
		set_flags(channel_name, CHANNEL_WIZ_ONLY);
		printf("  --> only wizards may tune in\n");
		break;

	    case "permanent":
		/* ### need better security? */
		if ( !adminp(user) )
		{
		    printf("Only admins can tweak permanent channels.\n");
		    return;
		}
		set_permanent(channel_name, 1);
		printf("  --> the channel is permanent\n");
		break;

	    case "nopermanent":
	    case "goaway":
		/* ### need better security? */
		if ( !adminp(user) )
		{
		    printf("Only admins can tweak permanent channels.\n");
		    return;
		}
		set_permanent(channel_name, 0);
		printf("  --> the channel may now go away\n");
		test_for_purge(channel_name);
		break;
	    }
	}

	/* tune the channel in now */
	arg = "/on";
    }
    if ( arg == "/on" )
    {
	if ( listening )
	{
	    printf("You are already listening to '%s'.\n", user_channel_name);
	}
	else if ( !ci )
	{
	    printf("'%s' does not exist. Use /new to create it.\n",
	      user_channel_name);
	    return;
	}
	else
	{
	    /* enforce the channel restrictions now */
	    /* ### not super secure, but screw it :-) */
	    if ( (ci->flags & CHANNEL_WIZ_ONLY) && !wizardp(user) )
	    {
		printf("Sorry, but '%s' is for wizards only.\n",
		  user_channel_name);
		return;
	    }
        if ( (ci->flags & CHANNEL_ADMIN_ONLY) && !adminp(user) && member_array(user->query_userid(), SECURE_D->query_domain_members("admin-channels")) == -1)
	    {
		printf("Sorry, but '%s' is for admins only.\n",
		  user_channel_name);
		return;
	    }
	user->add_channel(channel_name);
	printf("You are now listening to '%s'.\n", user_channel_name);
	}
	
	print_mod_info(channel_name);
	
	return;
    }

    /*
    ** All the following "commands" need to have the player listening
    */
    if ( !listening )
    {
	printf( "You are not listening to %s.\n", channel_name );
	return;
    }

    sender_name = user->query_name();

    if ( arg == "/off" )
    {
	user->remove_channel(channel_name);
	printf("You are no longer listening to '%s'.\n", user_channel_name);

	moderation_signoff(channel_name);
    }
    else if ( arg == "/list" || arg == "/who" )
    {
	tell(user, sprintf("Users listening to '%s': %s.\n",
	    user_channel_name,
	    make_name_list(ci->listeners)), MSG_INDENT);
    }
    else if( arg == "/last" || arg == "/history" )
    {
	string history = implode(ci->history, "");

	if ( history == "" )
	    history = "<none>\n";

	more(sprintf("History of channel '%s':\n%s\n",
	    user_channel_name, history));
    }
    else if( arg == "/clear" )
    {
	if ( adminp(user) || user = ci->moderator )
	{
	    ci->history = ({});
	    write("Channel cleared.\n");
	}
	else
	    error("Illegal attempt to clear channel " +
		  user_channel_name + " by " +
		  capitalize(this_body()->query_userid()) + ".");
    }
    else if ( cmd_moderation(channel_name, arg) )
    {
	/* it was handled */
    }
    else if ( arg[0] == ';' || arg[0] == ':' )
    {
	if ( ci->moderator && ci->speaker &&
	  user != ci->moderator && user != ci->speaker )
	{
	    printf("You are not the speaker on '%s'.\n", user_channel_name);
	}
	else if ( channel_type == CHANNEL_TYPE_IMUD )
	{
	    mixed * soul;
	    string the_soul;

	    if (sscanf(arg[1..],"%s %*s",the_soul)!=2)
		the_soul=arg[1..];
	    if (SOUL_D->query_emote(the_soul))
		soul = SOUL_D->parse_imud_soul(arg[1..]);
	    if ( soul )
		deliver_soul(channel_name, soul);
	    else
		deliver_emote(channel_name, sender_name, arg[1..]);
	}
	else
	{
	    mixed * soul;
	    string the_soul;

	    if (sscanf(arg[1..],"%s %*s",the_soul)!=2)
		the_soul=arg[1..];
	    if (SOUL_D->query_emote(the_soul))
		soul = SOUL_D->parse_soul(arg[1..]);
	    if ( soul )
		deliver_soul(channel_name, soul);
	    else
		deliver_emote(channel_name, sender_name, arg[1..]);
	}
    }
    else
    {
	if ( ci->moderator && ci->speaker &&
	  user != ci->moderator && user != ci->speaker )
	{
	    printf("You are not the speaker on '%s'.\n", user_channel_name);
	}
	else
	{
	    deliver_tell(channel_name, sender_name, arg);
	}
    }
}
