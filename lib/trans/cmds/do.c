/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// USAGE: do cmd1;cmd2;cmd3
//
// Does multiple commands

inherit CMD;

void main(string arg)
{
  string array cmds;

  if (!arg)
    return write("Syntax : do <cmd_1>;...;<cmd_n>\n");

  cmds=explode(arg,";");

  if (sizeof(cmds))
    map_array(cmds, (: this_user()->force_me($1) :));
}
