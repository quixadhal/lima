/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE:  rmdir < directory >
//
//This command will delete a directory if it is empty.
//
//rmdir /wiz/zifnab/obj
//
//will try and delete a directory named obj from /wiz/zifnab
//
//rmdir obj
//
//will try and delete a directory named obj from my current directory

inherit CMD;

private void main(mixed *arg)
{
  string dir;
  foreach(dir in arg[0])
  {
    if (rmdir(dir))
      outf("Removed directory: %s.\n", dir);
    else
      outf("Failed to remove directory: %s.\n", dir);
  }
}
