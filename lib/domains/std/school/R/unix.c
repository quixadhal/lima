#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Unix Commands");
    set_long(@LANCELOT
Basic Unix Commands (Write these down)
cd - change directory
ls - list the files in the current directory
pwd - show (print) working directory
more - view the specified file, with pauses
cat - view the specified file, without pauses
tail - view the last few lines of specified file
ed - edit the specified file
rm - removes file
mv - move or rename
mkdir - make directory
rmdir - remove directory
grep - search file(s) for specified text

Use 'help <command>' for further details
LANCELOT
);

    set_exits( ([
	"corridor" : R + "corridor02",
      ]) );
}
