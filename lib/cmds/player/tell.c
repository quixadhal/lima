/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust rewrote tell because it didn't do: tell rust@lima bean foo correctly.

#include <mudlib.h>
#include <commands.h>

inherit CMD;
inherit M_GRAMMAR;
inherit M_COMPLETE;
inherit M_ANSI;

void create()
{
    ::create();
    no_redirection();
}

private void main(string arg)
{
    string user;
    string host;
    string tmp;
    string * words;
    string muds;
    string * previous_matches;
    string * matches;
    int i, j;
    string mystring;
    string deststring;
    object who;

    if(!arg)
    {
	out("Usage: tell <user> <message>\n");
	return;
    }

    if(sscanf(arg,"%s@%s", user, tmp) == 2){
	muds = IMUD_D->query_up_muds();
	words = explode(tmp, " ");
	j = sizeof(words);
	tmp = "";
	for(i=0;i<j;i++)
	{
	    tmp += " " + words[i];
	    if(tmp[0] == ' ')
		tmp = tmp[1..];
	    matches = find_best_match_or_complete(tmp, muds);
	    if(!sizeof(matches))
	    {
		break;
	    }
	    previous_matches = matches;
	}
	if(previous_matches)
	{
	    if(sizeof(previous_matches) > 1)
	    {
		out("Vague mud name.  could be: " 
		  + implode(previous_matches, ", ") + "\n");
		return;
	    }                


	    host = previous_matches[0];
	    arg  = implode(words[i..], " ");
	    if(host == mud_name())
	    {
		main(user+" "+arg);
		return;
	    }

	    if( arg[0] == ';' || arg[0] == ':' )
	    {
		mixed *soul_ret;
		arg = arg[1..];
		soul_ret = SOUL_D->parse_imud_soul(arg);
		if(!soul_ret)  {
		    IMUD_D->do_emoteto(host, user, arg);
		    outf("You emote to %s@%s: %s %s\n", capitalize(user), host, this_body()->query_name(), arg);
		    return;
		}
		IMUD_D->do_emoteto(host,user,soul_ret[1][<1]);
		outf("*%s", soul_ret[1][0]);
		return;
	    }
	    IMUD_D->do_tell(host, user, arg);
	    outf("You tell %s@%s: %s\n", capitalize(user), host, arg);
	    return;
	}
    }
    if(sscanf(arg, "%s %s", user, arg) != 2)
    {
	out("Usage: tell <user> <message>\n");
	return;
    }
    who = find_body(lower_case(user));
    if(!who)
    {
	outf("Couldn't find %s.\n", user);
	return;
    }

    if (who->query_invis() && !adminp(this_user()) )
    {
	outf("Couldn't find %s.\n", user);
	return;
    }
    if (!who->query_link() || !interactive(who->query_link()))
    {
	outf("%s is linkdead.\n", who->query_name());
	return;
    }

    if( arg[0] == ':' || arg[0] == ';' )
    {
	mixed *soul_ret;
	int tindex;
	arg = arg[1..];
	soul_ret = SOUL_D->parse_soul(arg);
	if(!soul_ret)  {
	    mystring = sprintf("You emote to %s: %s %s\n", who == this_body() ? "yourself" : who->query_name(), this_body()->query_name(),arg);
	    deststring = sprintf("*%s %s\n", this_body()->query_name(), arg);
	}
	else
	{
	    mystring = sprintf("(tell) %s", soul_ret[1][0]);

	    if((tindex = member_array(who, soul_ret[0])) == -1)  {
		deststring = sprintf("(tell) %s", soul_ret[1][<1]);
	    }
	    else
	    {
		deststring = sprintf("(tell) %s", soul_ret[1][tindex]);
	    }
	}
    }
    else
    {
	mystring = sprintf("You tell %s: %s\n", who == this_body() ? "yourself" : who->query_name(), arg);
	deststring = "%^TELL%^" + this_body()->query_name() + " tells you: %^RESET%^" + arg + "\n";
    }

    out(mystring);
    if(who != this_body())
    {
	who->receive_private_msg(deststring, MSG_INDENT);
	who->set_reply(this_user()->query_userid());
    }
}

nomask int
valid_resend(string ob) {
    return ob == "/cmds/player/reply";
}
