/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private nosave mapping locks = ([]);

private nomask void unlock(string fname)
{
  map_delete(locks, fname);
}

nomask private void main(string* argv)
{
  mixed fname;

  if ( sizeof(argv) )
    fname = argv[0];
  if (objectp(fname))
    fname = base_name(fname);

  if ( !fname )
  {
    fname = get_user_variable("cwf");
    if ( !fname )
    {
      out("You have no cwf. Please specify a file.\n");
      return;
    }
  }
  else
    fname = evaluate_path(fname);

  if(objectp(locks[fname]))
  {
    printf("Sorry, that file is already being edited by %s.\n",
        locks[fname]->query_userid());
    return;
  }

  if(file_size(fname)>MAX_FILE_SIZE)
  {
    printf("Sorry, that file is too large to edit.\n");
    return;
  }

  locks[fname] = this_user();
  this_user()->query_shell_ob()->set_cwf(fname);

  new(ED_SESSION)->begin_editing(fname, 0, (: unlock($(fname)) :));
}
