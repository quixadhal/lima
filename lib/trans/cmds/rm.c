/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust

//:COMMAND
//USAGE:  rm < file >
//
//This command will delete the named file from the directory.
//If only a file name is given, rm assumes the file is in your
//current directory.
//
//rm /wiz/zifnab/harry.c
//
//will remove the file harry.c from my /wiz/zifnab
//
//rm harry.c
//
//will remove harry.c from my current directory if it exists.
//
//Wildcards can be used to select multiple files - eg *.c.

inherit CMD;

private void main(mixed argv)
{
  map(argv[0], (: rm($1) ? outf("%s: removed.\n", $1) :
		outf("failed to remove: %s\n", $1) :));
}
