/* Do not remove the headers from this file! see /USAGE for more info. */


//:PLAYERCOMMAND
//
// USAGE: tell <player> <message>
//        tell <player>@<mudname> <message>
//        tell /last
//        tell /clear
//
// This command is used to tell others private messages. The second format
// can be used to tell to people on other muds. The /last syntax will
// display your tell history and the /clear syntax will clear it.

#include <commands.h>
#include <playerflags.h>

inherit CMD;
inherit M_GRAMMAR;
inherit M_COMPLETE;
inherit M_ANSI;

#define MAX_HISTORY 20

void query_history(string name);
void add_history(string name, string msg);
void clear_history(string name);

private mapping history = ([ ]);

void create()
{
  ::create();
  no_redirection();
}

private void main(string arg)
{
  string user;
  string host;
  mixed tmp;
  string array words;
  string array muds;
  string array previous_matches;
  string array matches;
  int i, j;
  string mystring;
  string deststring;
  object who;

  if(!arg)
  {
    out("Usage: tell <user> <message>\n");
    return;
  }

  if(arg == "/clear")
  {
    this_user()->clear_tell_history();
    out("tell history cleared\n");
    return;
  }

  if (arg == "/last"|| arg == "/history")
  {
    string array out=({"History of tells:\n"});
    string array msgs;
    msgs=this_user()->list_tell_history();
    if(sizeof(msgs))
      out+=msgs;
    else
      out+=({"\t<none>"});
    more(out);
    return;
  }

  if(sscanf(arg,"%s@%s", user, tmp) == 2)
  {
/* Matching names with more than one word causes problems. */
    if(sizeof(explode(user," "))==1)
    {
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
/* We don't want to match EVERYTHING.  That's just...uck */
          if(sizeof(previous_matches)>10)
          {
            out("Vague mud name.  There are several possible matches.\n");
            return;
          }

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
          array soul_ret;
          arg = arg[1..];
          soul_ret = SOUL_D->parse_imud_soul(arg);

          if(!soul_ret)
          {
            IMUD_D->do_emoteto(host, user, "$N " + arg);
//				 sprintf("%s@%s %s",capitalize(user),host,arg));
            outf("%%^TELL%%^You emote to %s@%s:%%^RESET%%^ %s %s\n",
                capitalize(user),
                host,
                capitalize(this_user()->query_userid()),
                arg);
            this_user()->add_tell_history(sprintf("%%^TELL%%^You emote to %s@%s:%%^RESET%%^ %s %s\n",
                capitalize(user),
                host,
                capitalize(this_user()->query_userid()),
                arg));
            return;
          }
          IMUD_D->do_emoteto(host,user,soul_ret[1][<1]);
          outf("%%^TELL%%^(tell)%%^RESET%%^ %s", soul_ret[1][0]);
          this_user()->add_tell_history(sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s",
              soul_ret[1][0]));
          return;
        }
        IMUD_D->do_tell(host, user, arg);
        outf("%%^TELL%%^You tell %s@%s: %%^RESET%%^%s\n", capitalize(user), host, arg);
        this_user()->add_tell_history(sprintf("%%^TELL%%^You tell %s@%s: %%^RESET%%^%s\n",
            capitalize(user), host, arg));
        return;
      }
    }
  }

  if(sscanf(arg, "%s %s", user, arg) != 2)
  {
    out("Usage: tell <user> <message>\n");
    return;
  }
  who = find_user(lower_case(user));
  if(!who)
  {
    outf("Couldn't find %s.\n", user);
    return;
  }

  if (!interactive(who))
  {
    outf("%s is linkdead.\n", who->query_name());
    return;
  }

  if( arg[0] == ':' || arg[0] == ';' )
  {
    array soul_ret;
    int tindex;

    arg = arg[1..];

    soul_ret = SOUL_D->parse_soul(arg);
    if(!soul_ret)
    {
       mystring = sprintf("%%^TELL%%^You emote to %s: %%^RESET%%^%s %s\n",
           who == this_user() ? "yourself" :
           capitalize(who->query_userid()),
           capitalize(this_user()->query_userid()),
           arg);
       deststring = sprintf("*%s %s\n",
           capitalize(this_user()->query_userid()),
           arg);
    } else {
      mystring = sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s",
          soul_ret[1][0]);

      if((tindex = member_array(who, soul_ret[0])) == -1)
      {
        deststring = sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s",
            soul_ret[1][<1]);
      }	else {
        deststring = sprintf("%%^TELL%%^(tell)%%^RESET%%^ %s",
            soul_ret[1][tindex]);
      }
    }
  } else {
    mystring = sprintf("%%^TELL%%^You tell %s:%%^RESET%%^ %s\n",
        who == this_user() ? "yourself" :
        capitalize(who->query_userid()),
        arg);
    deststring = "%^TELL%^" + capitalize(this_user()->query_userid()) +
        " tells you: %^RESET%^" + arg + "\n";
  }

  if(who->query_body() && who->query_body()->test_flag(F_INACTIVE))
    mystring += sprintf("However %s is currently inactive, so don't expect "
        "a replay anytime soon.\n", capitalize(who->query_userid()));

  out(mystring);
  this_user()->add_tell_history(mystring);
  if(who != this_body())
  {
    who->receive_private_msg(deststring, MSG_INDENT);
    who->add_tell_history(deststring);
    who->set_reply(this_user()->query_userid());
  }
}

nomask int valid_resend(string ob){ return ob == "/cmds/player/reply"; }
