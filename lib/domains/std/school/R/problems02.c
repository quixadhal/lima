#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Runtime errors and dbxwhere");
    set_long(@LORIEL

There will be runtime errors when files have been checked and loaded OK,
but when running something goes wrong.

The error will be reported, with file name, line number and player/npc name.
It will also be logged.

A trace of what led up to the error can be obtained with 'dbxwhere <name>'

EXAMPLE:
Fri Feb 23 07:41:29 2001
*Attempted to read '\0' into string!
Object: /trans/cmds/tail at line 33

#0: '<function>' at <none> (<function>) at /:0
#1: 'dispatch_modal_input' at <none> (/secure/user/inputsys.c) at line 311
#2: '<function>' at <none> (<function>) at /:0
#3: 'shell_input' at <none> (/obj/secure/shell.c) at line 116
#4: 'execute_command' at <none> (/trans/obj/wish.c) at line 240
#5: 'call_main' at /trans/cmds/tail (obj/secure/cmd.c) at line 34
#6: 'main' at /trans/cmds/tail at line 33

LORIEL
    );
    set_exits( ([
	"backward" : R "problems01",
	"forward" : R "problems03",
      ]) );
    set_objects( ([
      ]) );
}
