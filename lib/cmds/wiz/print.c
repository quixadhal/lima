/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

private void main(string str, mapping flags, string stdin)
{
  if(stdin)
    out(stdin);
  else
    out(str+"\n");
}
