/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: loadall <dir>
//
//Removes and reloads all the files in a specified directory

/* To help with conversion testing */
#include <mudlib.h>
inherit CMD;

private void main(mixed * args)
{
  string * files;
  object * obs;

  outf("Attempting to load %s/*.c...  ", args[0]);

/*
 ** Get all the files in the given directory.  If this command is in
 ** the directory, then remove it... we don't want to dest self.
 */
  files = map_array(get_dir(args[0] + "/*.c"), (: $(args[0])+"/"+$1 :));
  files -= ({ file_name(this_object()) + ".c" });

/*
 ** Blow away the old objects
 */
  obs = map_array(files, (: find_object($1) :)) - ({ 0 });
  obs->remove();
  obs -= ({ 0 });
  map_array(obs, (: destruct($1) :));

/*
 ** Load all the objects
 */
  map_array(files, (: load_object($1) :));

  out("Succeeded.\n");
}



