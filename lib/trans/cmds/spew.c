/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$see : shell
//USAGE: spew file [<startline>] [#<lines>]
//
//Dumps contents of specified file to the screen as "says" from you,
//one say per line.
//Optional parameters for start line and number of lines to dump.

inherit CMD;

// Quick hack by Rust 12-12-93
private void main(mixed* arg) {
  string buf;
  string* lines;
  int startln, numlines;
  string pipe;
  string file;
  string rest;

  file = arg[0];
  if(arg[1])
  {
    rest = implode(arg[1]," ");
    sscanf(rest, "%d %d",startln, numlines);
  }

  buf = (numlines ? read_file(file, startln, numlines) : read_file(file, startln));

  if(!buf)
  {
    write("Couldn't find your file.\n");
    return;
  }
  pipe = "say";
  lines = explode(buf, "\n");
  foreach(string line in lines - ({ 0 }) - ({ "" }))
      this_user()->force_me(pipe+" "+line);
}

int help(){

    write("Usage: spew <file> [startline] [# lines] [| command]\n"
      "dumps contents of the file to the screen as a say from the user.\n"
      "if pipe is used, dumps to the command specified.  Ie, \n"
      "spew /cmds/xtra/_spew | wiz\n"
      "would send the contents of this file over the wiz line.\n"
      "\n");
}
