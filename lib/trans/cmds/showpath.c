/* Do not remove the headers from this file! see /USAGE for more info. */

/* Created by Ranma@Koko Wa 951001 */

/* This command will show you your current exec path */

//:COMMAND
//$$ see: addpath, rmpath
//USAGE: showpath
//
//This command will show you your current list of exec paths.

inherit CMD;

private void main()
{
  string * paths = this_user()->query_shell_ob()->query_path();

  out("Your current path is: " + implode(paths, (: $1 + ", " + $2 :)) + "\n");
}
