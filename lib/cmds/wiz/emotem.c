/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: emote, echom
//USAGE: emotem @<target> <message>
//
//Sends a targetted message, using the M_MESSAGE syntax (as for souls).
//As with other emotes, the senders name is prepended as part of the message.
//If the target cannot be found, it is ignored - ie the message is treated
//as a "simple_action" - so presence of $t in the message would cause error.
//
//
//>echom @fred $vtest the emotem command on $t
//
//>You test the echom command on Fred
//and Fred and onlookers would see suitably adjusted versions.

inherit CMD;
inherit M_GRAMMAR;

private void main(string args)
{
  string who;
  object targ;

  if (!sizeof(args))
  {
    write("Act how?\n");
    return;
  }

  sscanf(args, "@%s %s", who, args);
  args = punctuate("$N " + args);

  if(who)
  {
    targ = find_body(who);
    if(!targ)
      targ = present(who, this_body());
    if(!targ)
      targ = present(who, environment(this_body()));
  }

  if (targ)
  {
    this_body()->targetted_action(args, targ);
  } else {
    this_body()->simple_action(args);
  }
}
