/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: privs <pathname>
//
//Used to display privs (which for the basis of the Lima security system)- ie
// the privs needed by a wizard to read/write the specified directory.
//
//>privs /cmds/verbs
//
//Read: 0
//Write: "Mudlib:verbs"

inherit CMD;

void main(mixed input)
{
  mapping tmp;
  if(!stringp(input))
    input = "";
  tmp = SECURE_D->query_access_tree(evaluate_path(input), 0);
  printf("Read: %O\n", tmp[keys(tmp)[0]][0]);
  tmp = SECURE_D->query_access_tree(evaluate_path(input), 1);
  printf("Write: %O\n", tmp[keys(tmp)[0]][0]);
  return;
}
