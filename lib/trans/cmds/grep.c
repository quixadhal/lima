/* Do not remove the headers from this file! see /USAGE for more info. */

//  Grep - Locate files containing a particular regular expression.
//  Created by Rust, Jul 10, 1995
#include <mudlib.h>

inherit CMD;
inherit M_REGEX;
inherit M_GLOB;

private void 
stdin_grep(string pattern, mapping flags, string stdin)
{
  string array lines;
  int	bits = 0;
  int	i;

  if(!stdin)
    {
      out("Too few arguments.\nUsage: grep -inv string file(s)\n");
      return;
    }

  if(flags["i"])
	pattern = insensitive_pattern(pattern);
  if(flags["n"])
        bits |= 1;
  if(flags["v"])
        bits |= 2;
  lines = regexp(explode(stdin, "\n"),pattern, bits);
  if(!sizeof(lines))
    {  
      out("No matches.\n");
    }
  else
    {
      if(flags["n"])
	{
	  for(i = 0;i<sizeof(lines);i++)
	    {
	      outf(" %d  %s\n", lines[i+1], lines[i++]);
	    }
	}
      else
	{
	  map(map(lines,(:$1+"\n":)), (: out :));
	}
    }
}

private void
main(mixed argv, mapping flags, string stdin)
{
    string file;
    string this_output;
    string pattern;
    
    pattern = replace_string(argv[0], "/", "\\/");
    if(!argv[1])
      {
	stdin_grep(pattern, flags, stdin); 
	return;
      }
    if(flags["v"])
      {
	map(argv[1], (:stdin_grep($(pattern), $(flags),
				  read_file($1)):));
	return;
      }

    argv[1] = decompose(map(argv[1], (:is_directory($1) ?
	glob($1 + ($1[<1] == '/' ? "*" : "/*")): $1:)));

    if(flags["i"])
	pattern = insensitive_pattern(pattern);
    foreach(file in argv[1])
    {
	if(file_size(file) < 1)
	    continue;
	ed_start(file);
	if(flags["n"])
	    ed_cmd("n");
	this_output = ed_cmd("1,$g/"+pattern+"/p");
	if(this_output && strlen(this_output))
	    outf("[%s]:\n%s\n\n", file, this_output);
	ed_cmd("q");
    }
    if(!strlen(get_output()))
	out("No matches found.\n");
}
