// Beek, End of July

#include <mudlib.h>
inherit CMD;

private void main(string arg) {
    if (!arg) {
	write("Nickname yourself what?\n");
	return;
    }
    
    this_body()->set_nickname(arg);
}

