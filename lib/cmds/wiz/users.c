/* Do not remove the headers from this file! see /USAGE for more info. */

/* List the user names of all people listed in the "users()" array
 *
 * Peregrin@ZorkMUD
 */

 //:COMMAND
 //USAGE:  users
//
//This command lists the names of all users currently connected.
//It is convenient for those who don't wish to clutter their screen with excess
//information such as what comes with the "who" command.

inherit CMD;

private void main()
{
  object array ulist = users();
  string ustring;

  if (sizeof(ulist) <= 0)
  {
    out(sprintf("NO ONE LOGGED IN!!!   (not even you!)\n"));
    return;
  }
  if ((sizeof(ulist)==1) && (ulist[0]==this_user()))
  {
    out(sprintf("You are the only person logged in!\n"));
    return;
  }
  if(end_of_pipeline())
    out(implode(ulist->query_userid(),
        (: $1 + " " + ($2 ? $2 : "(login)") :),"")[1..] + "\n");
  else
    out((implode(ulist->query_userid(),
        (: $1 + "\n" + ($2 ? $2 : "(login)") :), "")[1..] + "\n"));
}