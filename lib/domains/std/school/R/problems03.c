#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Further information from dbxframe");
    set_long(@LORIEL
Each of the steps which led up to an error can be shown with dbxframe.
This will display a few lines of code either side, and variable values.
Using the previous example, and abbreviating the display:
'dbxframe 3'
------/obj/secure/shell.c:116 - shell_input("tail execute")
locals:
---------------------------------------------------------------
        input = input[1..];
    else
        input = expand_alias(input);
    if ( input != "" )
=>      execute_command(input);
}

LORIEL
    );
    set_exits( ([
	"backward" : R "problems02",
	"corridor" : R "corridor09",
      ]) );
    set_objects( ([
      ]) );
}
