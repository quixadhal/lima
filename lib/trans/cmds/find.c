/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: findfile, grep
//USAGE: find <pattern> <files>
//
//Display lines containing match to specified target pattern, within specified files.

//  Created by Rust, Jul 10, 1995

inherit CMD;
inherit M_REGEX;
inherit M_GLOB;

private void
main(mixed argv, mapping flags)
{
  string file;
  string this_output;
  argv[1] = decompose(map(argv[1], (:is_directory($1) ?
			  glob($1 + ($1[<1] == '/' ? "*" : "/*")): $1:)));

  if(flags["i"])
    argv[0] = insensitive_pattern(translate(argv[0],1));
  foreach(file in argv[1])
  {
    ed_start(file);
    if(flags["n"])
      ed_cmd("n");
    this_output = ed_cmd("1,$g/"+argv[0]+"/p");
    if(this_output && strlen(this_output))
      outf("[%s]:\n%s\n\n", file, this_output);
    ed_cmd("q");
  }
  if(!strlen(get_output()))
    out("No matches found.\n");
}
