/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_GLOB;
inherit M_REGEX;


int main(string arg)
{
  mapping mudlist = IMUD_D->query_mudlist();
  string* muds = keys(mudlist);
  string* matches;
  string  match;
  mixed*  mudinfo;
  string  output;

  if(!arg || arg == "") matches = muds;
  else
    {
      matches = insensitive_regexp(muds, translate(arg));
      if(!sizeof(matches))
	{
	  printf("No muds out of %d match that pattern.\n", sizeof(mudlist));
	  return 1;
	}
    }
  output = sprintf("%d matches out of %d muds.\n"
"Type mudinfo <mudname> for more info on a mud.\n"
"Up Mud              Address                 Mud Type   Open Status\n"
"------------------------------------------------------------------\n", 
sizeof(matches), sizeof(mudlist));

  matches = sort_array(matches,1);
  foreach(match in matches)
    {
      mudinfo = mudlist[match];
      output += sprintf("%c  %-15s  %-16s %-5d  %-8s  %-20s\n", 
			mudinfo[0] == -1? 'U' : 'D', match,
			mudinfo[1], mudinfo[2], mudinfo[8], mudinfo[9]);
    }
  
  clone_object(MORE_OB)->more_string(output);
  return 1;
}
			

