/* Do not remove the headers from this file! see /USAGE for more info. */

// Simple cp, no -r yet.

//:COMMAND
//USAGE:  cp  <source> <destination>
//
//This command will allow you to copy files from source to
//destination.  The source file must exist, the destination may or
//may not exist at the time of the copy.  If the destination does
//not exist one will be created, if the destination does exist, at
//the time of the copy it will be overwritten.
//
//The optional flag f can be used to force it to overwrite -
//
//cp -f wizroom.c workroom.c
//
//If you do not supply a full path name cp will assume that the
//files will be in your current directory.
//
//cp wizroom.c workroom.c
//
//I will now have a copy of wizroom.c named workroom.c in the
//  current directory.
//
//cp /domains/std/wizroom.c /wiz/zifnab/workroom.c
//
//I now have a copy of wizroom.c in my home directory named workroom.c
//
//Like the *nix cp command, it can also be used with multiple source files
//(eg using wildcard) provided the destination is a directory - eg
//
//cp s*.c /wiz/loriel/rooms/
//
//This copies all files in current directory, which have a .c suffix, and
// which start with s, to the directory /wiz/loriel/rooms/.

inherit CMD;

private void copy_one(string src, string dst, int force)
{
  string contents;

  if ( is_file(dst) && !force )
    outf("%s already exists.  Copy failed.\n", dst);
  else if ( !(contents = read_file(src)) )
    outf("Could not read %s.  Copy failed.\n", src);
  else if ( !write_file(dst, contents, 1) )
    outf("%s could not be written to.  Copy failed.\n", dst);
  else
    outf("%s copied to %s.\n", src, dst);
}

private void main(mixed argv, mapping flags)
{
  string src;
  string dst;

  if(sizeof(argv[0]) > 1 && !is_directory(argv[1]))
  {
    outf("cp: files dir not files file\n");
    return;
  }
  if(sizeof(argv[0]) > 1)
  {
    if(argv[1][<1] != '/')
      argv[1] += "/";

    foreach(src in argv[0])
    {
      dst = argv[1] + split_path(src)[1];
      copy_one(src, dst, flags["f"]);
    }
  } else {
    dst = argv[1];
    src = argv[0][0];

    if(is_directory(dst))
    {
      if(dst[<1] != '/')
        dst += "/";
      dst += depath(src);
    }

    copy_one(src, dst, flags["f"]);
  }
}
