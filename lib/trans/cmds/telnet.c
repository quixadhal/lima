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

#include <mudlib.h>
inherit CMD;

private void main()
{
  write("Type /help or /? for help on telnet.\n");
  new(TELNET_OB)->init_telnet();
}
