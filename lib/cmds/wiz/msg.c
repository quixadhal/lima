/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: m_messages, review
//USAGE:  msg <message type> clear
//        msg <message type> <message>
//
//This command allows you to change your default messages.  You can
//add more than one message for each message type.  When you use the
//matching command (i.e. clone ) one of the messages will be
//choosen at random from the appropriate messages.
//
//You can also remove all the messages of a given type by using the "clear"
//option.  You will not be able to remove a particular message from a group
//of messages.
//
//msg clone clear
//
//     This will remove all messages from the type clone.
//
//msg clone $N $vreach back to $p workroom and $vgrab $o.
//
//     Next time you use clone, you will see the above message.
//     If you do the above again you will add another message.

inherit CMD;

private void main(mixed arg)
{
  string value;

  value = implode(arg[1], " ");
  if ( value == "clear" )
    this_body()->set_msgs(arg[0], ({}));
  else
    this_body()->add_msg(arg[0], value );

  out("Ok.\n");
}
