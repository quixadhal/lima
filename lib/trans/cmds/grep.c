/* Do not remove the headers from this file! see /USAGE for more info. */

//  Grep - Locate files containing a particular regular expression.
//  Created by Rust, Jul 10, 1995
#include <mudlib.h>

inherit DAEMON;
inherit M_REGEX;
inherit M_GLOB;

int
main(mixed argv, mapping flags)
{
  string file;
  string output = "";
  string this_output;
  argv[1] = decompose(map(argv[1], (:is_directory($1) ?
			  glob($1 + ($1[<1] == '/' ? "*" : "/*")): $1:)));

  if(flags["i"])
    argv[0] = insensitive_pattern(argv[0]);
  foreach(file in argv[1])
    {
      if(file_size(file) < 1)
	continue;
      ed_start(file);
      if(flags["n"])
	ed_cmd("n");
      this_output = ed_cmd("1,$g/"+argv[0]+"/p");
      if(this_output && strlen(this_output))
	output += sprintf("[%s]:\n%s\n\n", file, this_output);
      ed_cmd("q");
    }
  if(!strlen(output))
    output = "No matches found.\n";
  clone_object(MORE_OB)->more_string(output);
}
