/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//usage: wc [-lw] <filename(s)>
//
//Gives wordcount (and linecount) of the file specified.
//The 2 flags specify what to count :
//-l = linecount only
//-w = wordcount only

inherit CMD;
inherit M_REGEX;

private string count(string s, int flags)
{
 switch(flags)
   {
   case 1:
     return sprintf("%d\n", sizeof(split(s,"[ \n\t]+")));
   case 2:
     return sprintf("%d\n", sizeof(explode(s,"\n")));
   default:
     return sprintf("%d words, %d lines\n", sizeof(split(s,"[ \n\t]+")),
		    sizeof(explode(s,"\n")));
   }
}
   

private string count_file(string fname, int flags)
{
  return count(read_file(fname), flags);
}

private void main(mixed array argv, mapping flags, string stdin)
{
  int	f = 3;

  if(sizeof(flags)==1)
    {
      if(flags["w"])
	f = 1;
      else
	f = 2;
    }
  if(stdin)
    {
      out(count(stdin,f));
      return;
    }
  else
    {
      if(!sizeof(argv[0]))
	{
	  write("Usage: wc [-lw] file(s)\n");
	  return;
	}
      map(argv[0], (:out($1+":  "+count_file($1,$(f))):));
    }
}
