/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust rewrote tell because it didn't do: tell rust@lima bean foo correctly.

#include <mudlib.h>
#include <commands.h>

inherit CMD;
inherit M_GRAMMAR;
inherit M_COMPLETE;
inherit M_ANSI;

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
	write("Usage: tell <user> <message>\n");
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
	    matches = case_insensitive_complete(tmp, muds);
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
		write("Vague mud name.  could be: " 
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
	
	    if(arg[0] == ':')  {
		arg = arg[1..];
		IMUD_D->do_emoteto(host, user, arg);
		printf("You emote to %s@%s: %s %s\n", capitalize(user), host, this_body()->query_name(), arg);
	    
		return;
	    }
	    if(arg[0] == ';')  {
		mixed *soul_ret;
		arg = arg[1..];
		soul_ret = SOUL_D->parse_imud_soul(arg);
		if(!soul_ret)  {
		    write("No such soul.\n");
		    return;
		}
		IMUD_D->do_emoteto(host,user,soul_ret[1][<1]);
		printf("*%s", soul_ret[1][0]);
		return;
	    }
	    IMUD_D->do_tell(host, user, arg);
	    printf("You tell %s@%s: %s\n", capitalize(user), host, arg);
	    return;
	}
    }
    if(sscanf(arg, "%s %s", user, arg) != 2)
    {
	write("Usage: tell <user> <message>\n");
	return;
    }
    who = find_body(lower_case(user));
    if(!who)
    {
	printf("Couldn't find %s.\n", user);
        return;
    }

    if (who->query_invis() && !adminp(this_user()) )
    {
	printf("Couldn't find %s.\n", user);
	return;
    }
    if (!who->query_link() || !interactive(who->query_link()))
    {
	printf("%s is linkdead.\n", who->query_name());
	return;
    }
    
    if(arg[0] == ':')  {
        arg = arg[1..];
        mystring = iwrap(sprintf("You emote to %s: %s %s\n", who == this_body() ? "yourself" : who->query_name(), this_body()->query_name(),arg));
        deststring = iwrap(sprintf("*%s %s\n", this_body()->query_name(), arg));
    } else if(arg[0] == ';')  {
        mixed *soul_ret;
        int tindex;
        arg = arg[1..];
        soul_ret = SOUL_D->parse_soul(arg);
        if(!soul_ret)  {
            write("No such soul.\n");
            return;
        }
        mystring = iwrap(sprintf("(tell) %s", soul_ret[1][0]));
       
        if((tindex = member_array(who, soul_ret[0])) == -1)  {
            deststring = iwrap(sprintf("(tell) %s", soul_ret[1][<1]));
        } else {
            deststring = iwrap(sprintf("(tell) %s", soul_ret[1][tindex]));
        }
    } else {
	mystring = iwrap(sprintf("You tell %s: %s\n", who == this_body() ? "yourself" : who->query_name(), arg));
	deststring = ansi(iwrap("%^BOLD%^" + this_body()->query_name() + " tells you: %^RESET%^" + arg + "\n"), who);
    }

    write(mystring);
    if(who != this_body())
    {
	who->receive_private_msg(deststring);
	who->set_reply(this_user()->query_userid());
    }
}

nomask int
valid_resend(string ob) {
    return ob == "/cmds/player/reply";
}
 
