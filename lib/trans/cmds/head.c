/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: tail, more, ed
//USAGE: head [-n<number>] <file(s)>
//
//Prints out the top few lines of a file (or multiple files).
//Use the -n flag to specify different value for number of lines displayed -
//default = 10.


inherit CMD;

void tail(string file, int n)
{
  out(implode(explode(read_file(file),"\n")[0..(n-1)],"\n"));
}

private void main( mixed *arg, mapping flags, string stdin ) {
  int i;
  int n = 10;

  if(flags["n"])
    n = (to_int(flags["n"]) || 10);

  if(stdin)
  {
    out(implode(explode(stdin,"\n")[0..(n-1)],"\n"));
    return;
  }

  for (i=0; i<sizeof(arg[0]); i++)
    tail(arg[0][i],n);
}