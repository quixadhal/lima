/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: echo, echoall, m_messages
//USAGE echo @<target> <message>
//
//Provides a targetted echo facility, which uses the "message" syntax.
//If the target cannot be found, it is ignored - ie the message is treated
//as a "simple_action" - so presence of $t in the message would cause error.
//
//
//>echom @fred $N $vtest the echom command on $t
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
  args = punctuate(args);
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
    this_body()->simple_action(args, who);
  }
}
