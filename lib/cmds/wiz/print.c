/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE print <string>
//
//Rarely used as "print", but extremely useful in the aliased form @
//(or eval) for print `$*` which evaluates the string and displays the result.
//
//>@ANSI_D
//
//would return /daemons/ansi_d.c

inherit CMD;

private void main(string str, mapping flags, string stdin)
{
  if(stdin)
    out(stdin);
  else
    out(str+"\n");
}
