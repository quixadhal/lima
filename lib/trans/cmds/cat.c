/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$see: more,tail
//USAGE cat [path/file]
//
//Dumps the contents of the named file to your screen with no page breaks.
//If no file name is given cat assumes the last file edited.
//
//cat /wiz/zifnab/backup_menu.c

inherit CMD;

// Rust

private void main(mixed *arg, mapping flags, string stdin)
{
  string file;
  int i;
  mixed chunk;

  if(arg[0])
  {
    foreach(file in arg[0])
    {
      i = 0;
      if(!is_file(file))
	printf("Warning: cat of %s failed.\n", file);
      else
      {
        int place=0;
        while( (chunk=read_file(file,place,100))!=0 && chunk != "")
	{
          place += 101;
          out(chunk);
        }
      }
    }
  }
  else
    out(stdin);
}