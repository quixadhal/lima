/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_GLOB;
inherit DAEMON;

int main(mixed* argv, mapping flags)
{
  string file;
  string output = "";

  argv[0] = translate(argv[0], 1);

  foreach(file in argv[2])
    {
      ed_start(file);
      if(flags["n"])
	ed_cmd("set number");
      output = ed_cmd("1,$g/"+argv[0]+");
      if(output && strlen(output))
	{
	  output += sprintf("[Changing in %s]:\n%s\n\n", file, output);

	  ed_cmd(sprintf("1,$s/%s/%s/g", argv[0], argv[1]));
	  ed_cmd("w");
	}
      ed_cmd("q");

    }

  if(!strlen(output))
    write("No matches found.\n");
  else
    {
      if(flags["q"])
	printf("Done.\n");
      else new(MORE_OB)->more_string(output);
    }
}
