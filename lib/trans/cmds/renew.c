/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
// Usage: renew <object>
//
// Takes the specified cloned object, dests it, updates its base
// file (NOT its inherits), reclones it and replaces it to its
// original environment.

#include <commands.h>

inherit CMD;

private void main(mixed *arg)
{
  object ob, env;
  string file;
  string data;

  ob = arg[0];
  file = base_name(ob);

  if( file_size(file + ".c") < 0)
  {
    outf("renew: file '" + file + "' does not exist (or is a directory).\n");
    return 0;
  }
  env = environment(ob);
  data = ob->save_to_string(1);
  ob->remove();
  outf("Updating '" + file + "' ...\n");
  if (CMD_OB_UPDATE->do_update(file, 3) < time())
    out(file + ": No update necessary.\n");
  ob = new(file);
  ob->load_from_string(data, 1);
  ob->move(env);
}
