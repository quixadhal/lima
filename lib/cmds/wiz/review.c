/* Do not remove the headers from this file! see /USAGE for more info. */

// Rewritten by Beek
// And converted again by Rust

//:COMMAND
//$$ see: m_messages, msg
//USAGE:  review
//        review < playername >
//
//This command with no arguments will show you all your messages.
//It can also be used to display the messages of another player,
//by using "review <name>" where <name> is that player's name (lower case).
//
//review
//
//leave: [$N $vleave $o] You leave somewhere
//destruct: [$N $vdest the $o.] You dest the something.
//enter: [$N $venter.] You enter.
//clean: [$N $vdo not have a message for 'clean'.] You do not have a message for '
//clean'.
//home: [$N $vgo home.] You go home.
//clone: [$N $vclone the $o.] You clone the something.
//vis: [$N $vfade into view.] You fade into view.
//invis: [$N $vfade from view.] You fade from view.
//menter: [$N $vappear in a puff of smoke.] You appear in a puff of smoke.

inherit CMD;
inherit M_MESSAGES;

private void main(mixed *arg, mapping flags)
{
  int i,j;
  object ob;
  string *msg;
  string *msgs;
  string *ob_msgs;
  int debug;
  int flag;

  if (flags["d"])
    debug = 1;

  ob = arg[0];
  if (!ob)
    ob = this_body();

  msg = ob->query_msg_types();
  msg = filter(msg, (: stringp :));
  for ( i = 0; i < sizeof(msg); i++ )
  {
    if (debug)
      out("["+msg[i]+"]");
    ob_msgs = ob->query_msg(msg[i]);
    if (!stringp(ob_msgs) && !arrayp(ob_msgs))
      continue;
    if (!arrayp(ob_msgs))
      ob_msgs = ({ ob_msgs });

    for ( j = 0; j < sizeof(ob_msgs); j++ )
    {
      if (debug)
        out("["+ob_msgs[j]+"]");

      switch ( msg[i] )
      {
        case "leave":
          msgs = action( ({ ob }), ob_msgs[j], "somewhere");
          break;

        case "clone":
        case "destruct":
          msgs = action( ({ ob }), ob_msgs[j], "something");
          break;

        case "fol_leave":
        case "fol_enter":
          msgs = action( ({ ob, this_object() }), ob_msgs[j], "somewhere");
          break;

        default:
          msgs = action( ({ ob }), ob_msgs[j]);
      }

      out(msg[i] + ": [" + ob_msgs[j][0..<2] + "] ");
      if (ob == this_body())
        out(msgs[0]);
      else
        out(msgs[<1]);

      flag = 1;
    }
  }

  if (!flag)
    out("No messages.\n");
}

// so we can use this object as a generic object

string query_objective() { return "him"; }

string query_subjective() { return "he"; }

string query_named_possessive() { return "someone's"; }

string query_possessive() { return "his"; }

string short() { return "someone"; }

string a_short() { return short(); }

string query_reflexive() { return "somebody"; }