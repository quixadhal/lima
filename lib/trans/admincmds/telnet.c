/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** This file is part of the Lima distribution mudlib.
** Do not remove this header.  Please see /USAGE for more
** information on using this code.
**
** FILE: /trans/cmds/telnet.c
** AUTHOR: John Viega (rust@virginia.edu)
** CREATED: Jul 25 1995

** OVERVIEW
** ========
** This command is usable by anyone with valid security clearence to use sockets.
** It initiates /trans/obj/telnet_ob.
**
*/

//:ADMINCOMMAND
//USAGE: telnet
//
//Invokes the mud's telnet client for you.
//
//Admin only
//The prompt will change to #, meaning you're in telnet mode.
//Any input not directly to the telnet program will go to your
//average shell instead.
//
//Text received from a telnet session will start with the tilde  ( '~' ).
//
//Telnet understands the following commands:
//
///exit  -- quit telnet
//
///open [address|mudname]  -- opens a new telnet session.
//			    mudname can be any mud name or a unique part
//			    of a mud name based on the muds you can see
//			    by typing: "mudlist".
//			    Right now if you use an address, it must be
//			    an ip number, and not an ip name.
//	 	            If you have more than one session open,
//			    you'll only be interacting with the current
//			    session.
//
///jobs  -- shows what sessions you have open.
//
///goto [job#] -- changes your active session to the given number
//		(if you have more than one session)
//
///tog   -- toggles whether or not your regular input should go to the active
//	  session or to the mud you're on.  By default it goes to the mud
//	  you're on.
//
///send <str>  -- send input to the active session.  You don't need to do this
//		if you've done /tog once, you can just type the input and
//		it goes.
//
///close -- closes the currently active session.
//
///hide  -- You don't see any of the text from remote telnet connections if
//	  you use this.  /hide again to bring the text back.
//
///help		-- This document.
///?
//
//! as the first character always sends your input to your command shell
//instead of a telnet session.  To send a string to a telnet session that
//starts with a !, use '/send !string'.
//
//*N.B.  Some people have a hard time using / with tinyfugue.  If you need to
//       change the control character, set the telnet_char environment variable.
//	EG:
//		set telnet_char %

inherit CMD;

private void main()
{
  if (!check_privilege(1))
  {
    out("Only admins may use telnet!\n");
    return;
  } else {
    write("Type /help or /? for help on telnet.\n");
    new(TELNET_OB)->init_telnet();
  }
}
