// _snoopable.c
// this space intentionally left blank
// Megaboz@ZorkMUD added header and help

#include <playerflags.h>
#include <mudlib.h>

inherit CMD;

private void main(string arg){
  if(!arg || arg == ""){
    if(this_body()->test_flag(F_SNOOPABLE) )
		write("Joe Wizard can snoop you.\n");
	else write( "Joe wizard can't snoop you.\n");
    return;
  }
  switch(arg){
    case "on":  this_body()->set_flag(F_SNOOPABLE);
                break;
    case "off": this_body()->clear_flag(F_SNOOPABLE);
                break;
    default:    notify_fail("Syntax: snoopable (on|off)\n");
                return;
  }
  write("Ok.\n");
  return;
}
