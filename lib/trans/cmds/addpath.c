/* Do not remove the headers from this file! see /USAGE for more info. */

/* Created by Ranma@Koko Wa 951001 */

//:COMMAND
//$$ see: showpath, rmpath
//Usage: addpath [directory]
//
//This command will add a new path to your current exec path,
//which the command finder will search through.

inherit CMD;

#include <commands.h>

private void main(string * arg)
{
  string userid = this_user()->query_userid();
  string * paths;
  string item;
  object shell_ob = this_user()->query_shell_ob();

  if ( get_privilege(this_user()) && !check_privilege(userid) )
  {
    out("Permission denied.\n");
    return;
  }

//REPLACE THIS WITH A MORE GENERALISED CHECKING SYSTEM IF YOU HAVE MORE
//"RESTRICTED" COMMAND DIRECTORIES  
  if((arg[0] == TRANS_ADMINCMD_DIR) &&(!adminp(this_user())))
  {
    out("Need to be an admin to add that path!\n");
    return;
  }

  paths = shell_ob->query_path();
  foreach ( item in paths )
  {
    if ( item == arg[0] )
    {
      out("You have had the path added already\n");
      return;
    }
  }

  paths += ({ arg[0] + "/" });
  shell_ob->set_variable("path", paths);

  out(implode(paths,
	(: $1 + ($2[<1] == '/' ? $2[0..<2] : $2) + ", " :),
	"Your path is now: ")[0..<3] + "\n");
}
