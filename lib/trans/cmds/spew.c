/* Do not remove the headers from this file! see /USAGE for more info. */

// specification is: spew file str*
// we will recieve: ({ string, ({ args }) })
#pragma strict_types
#include <mudlib.h>

inherit DAEMON;
// Quick hack by Rust 12-12-93
int main(mixed* arg) {
    string buf;
    string* lines;
    string name;
    int loop;
    int i;
    int startln, numlines;
    string pipe;
    string file;
    string rest;

    file = arg[0];
    rest = implode(arg[1]," ");
    sscanf(rest, "%s | %s", rest, pipe);
    sscanf(rest, "%s %d %d", rest, startln, numlines);
    if(!buf = (numlines ? read_file(file, startln, numlines) :
	read_file(file, startln))){
	write("Couldn't find your file.\n");
	return 0;
    }
    if(!pipe) pipe = "say";
    lines = explode(buf, "\n");
    i = sizeof(lines);
    for(loop = 0; loop<i; loop++){
	this_user()->force_me(pipe+" "+lines[loop]);
    }
    return 1;
}

int help(){

    write("Usage: spew <file> [startline] [# lines] [| command]\n"
      "dumps contents of the file to the screen as a say from the user.\n"
      "if pipe is used, dumps to the command specified.  Ie, \n"
      "spew /cmds/xtra/_spew | wiz\n"
      "would send the contents of this file over the wiz line.\n"
      "\n");

    return 1;
}
