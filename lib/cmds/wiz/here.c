/* Do not remove the headers from this file! see /USAGE for more info. */

// _here.c   (wizard command)
// Peregrin@ZorkMUD

// just a temporary command (maybe) to let a wiz know the filename
// and short of a room.

#include <mudlib.h>
inherit CMD;



private void main() {
   object env;
   env = environment(this_body());
   printf("%s: [%s]\n",env->short(),file_name(env));
}
