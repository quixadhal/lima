// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 10");
  set_long(@MAY
   The SUBSTITUTE command may be THE most important command
that you will learn in ED.  It is the command that you use
to change text on a single line.  This is important because
this command saves you from having to type over an entire line.
Lets say that you have a line that starts off:  set_logn(...
and you want to change it to set_long(...  What you would type
is <s/logn/long>.  This SUBSTITUTES in 'long' for 'logn'.  You
need not use front slashes between the 2 strings being changed.
You can use any character as long as you only use one single
character.  For example, we can write out the first example
as <s!logn!long>.  This will accomplish exactly the same thing.
This is important when you are working with text of directories,
text that has a frontslash in it.  You would not want to use the
frontslash so you could use <s!/name/dir!/name/dir2>.  In this
case, the '!' is the character that divided the strings and the
slash is used in the string.  You may also want to use the 's'
command over a set number of lines.  In that case you would
write <1,25s/str1/str2>.  This will change the first occurance
of str1 on each line to str2.  If you want to change ALL
occurances of str1 to str2 you would have to add an extra
'g' command to the original text so it would look like
this: <1,25s/str1/str2/g>.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor09",
    "north" : R + "edtutor11",
   ]) );
  set_objects( ([
   ]) );
}
