/* Do not remove the headers from this file! see /USAGE for more info. */

// _here.c   (wizard command)
// Peregrin@ZorkMUD

// Note: after some textual analysis, Rust and I have determined the parts
// of this file which might be original (other than comments).
// It goes as follows:
// ... () { ... object env; env = environment( ... ()); ...
// "%s: [%s]\n", env->short(), file_name(env)); }

// just a temporary command (maybe) to let a wiz know the filename
// and short of a room.

// Temporary?  Funny how long some things last .... (Beek, about
// 2 years later)

//:COMMAND
//USAGE:  here
//
//This command shows you the pathname to the file of the object
//you are currently in.
//
//here
//
//Grand Hall: [/domains/std/wizroom

inherit CMD;

private void main()
{
  object env;
  env = environment(this_body());
  outf("%s: [%s]\n",env->short(),file_name(env));
}
