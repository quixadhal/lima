/* Do not remove the headers from this file! see /USAGE for more info. */

//:PLAYERCOMMAND
// Usage : rows <number> (to set the number of rows before pause in display)
//         rows          (to see current setting)

inherit CMD;

private void main(mixed rows)
{
  if(wizardp(this_user()))
    write("Wizards can also set/read the \"MORE\" variable directly\n"
          "using the \"set\" command\n"
          "eg \"set MORE 99\"\n");

  if(!stringp(rows))
  {
    printf("Number of rows currently set to %O\n",
        this_user()->query_shell_ob()->get_variable("MORE")
        );
    return;
  }
  if(stringp(rows) && (rows == "help"))
  {
    write("Usage : rows <number>\n"
          "to set the number of rows before pause in display.\n"
          );
    return;
  }
  rows = to_int(rows);
  this_user()->query_shell_ob()->set_variable("MORE", rows);
  printf("Setting number of rows to %d\n", rows);
}
