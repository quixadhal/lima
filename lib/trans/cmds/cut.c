/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE cut -f<from>[-<to>] [-d<delimiter>] <file(s)>
//
//Displays the specified file, each line being trimmed to a selection of words
//according to the -f flags.
//Each line is split into words - using the usual delimiters of space/tab etc.
//If only one valus is provided as an f flag, that "word" is displayed on each line.
//If 2 values are provided, words numbered from first value to second are displayed.

inherit CMD;
inherit M_REGEX;

private void cut_it(string what, string array fields, string delim)
{
  string array lines;
  string array delimited;
  int array rfields = ({ });
  string tmp;
  string output = "";
  string line;
  int hi,low;
  int i,val;
  int pos;
  int where;

  foreach(tmp in fields)
  {
    if(sscanf(tmp, "%d-%d", low, hi) == 2)
    {
      for(i = low; i <= hi; i++)
        rfields += ({ i-1 });
    } else if(sscanf(tmp, "%d", val) == 1) {
      rfields += ({ val -1});
    }
  }

  lines = explode(what, "\n");
  foreach(line in lines)
  {
    delimited = regexplode(line, delim);
    foreach(pos in rfields)
    {
      if(pos*2+1 >= sizeof(delimited))
        continue;
      where = member_array(pos, rfields);
      output += sprintf("%s%s",delimited[pos*2], (where < sizeof(rfields)-1) ? delimited[(pos*2)+1]:"");
    }
    output += "\n";
  }
  out(output + "\n");
  return;
}

private void main(mixed *argv, mapping flags, string stdin)
{
  string delim = "[ \r\t\n]+";
  string tmp;
  string array fields;
  string array filenames;
  string  contents;
  string file;

  if(!(tmp = flags["f"]))
  {
    printf("Must specify fields with the -f option.\n");
    return;
  }

  fields = explode(tmp, ",");
  if(flags["d"])
    delim = flags["d"];
  if(!argv[0])
  {
    if(!sizeof(stdin))
    {
      printf("Nothing to cut!\n");
      return;
    }
    cut_it(stdin, fields ,delim);
    return;
  }

  filenames = argv[0];
  foreach(file in filenames)
  {
    contents = read_file(file);
    if(!sizeof(contents))
      continue;
    cut_it(contents,fields,delim);
  }
  return;
}