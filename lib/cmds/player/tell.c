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
    printf("No such player.\n");
    return;
  }
  if (!who->query_link() || !interactive(who->query_link()))
    {
      printf("%s is linkdead.\n", who->query_name());
      return;
    }
    if(who != this_body())
      {
	write(iwrap(sprintf("You tell %s: %s\n", who->query_name(), arg)));
	tell_object(who, ansi(iwrap("%^BOLD%^" + this_body()->query_name() + " tells you: %^RESET%^" + arg + "\n"), who));
      }
  else
    write("You tell yourself: "+arg+"\n");
    who->set_reply(this_user()->query_userid());
}
     
nomask int
valid_resend(string ob) {
    return ob == "/cmds/player/reply";
}
 
