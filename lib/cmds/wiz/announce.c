/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz

#include <mudlib.h>
#include <playerflags.h>




int main(string arg) {
	if(!arg || !stringp(arg)){
		printf("Your announce flag is %s.\n",
(this_body()->test_flag(F_ANNOUNCE) ? "on" : "off"));
		return 1;
	}
	if(arg == "on"){
		write("Your announce flag is now on.\n");
		this_body()->set_flag(F_ANNOUNCE);
		return 1;
	}
	if(arg == "off"){
		write("Your announce flag is now off.\n");
		this_body()->clear_flag(F_ANNOUNCE);
		return 1;
	}
	write("Usage: announce [on | off]\n");
}

int help()
{
  write(wrap(
"Usage: announce [on | off]\n"
"The announce flag, if turned on, will give you messages as "
"people enter and leave the game.  Without an argument, this "
"command shows you the status of your announce flag.\n"
));
return 1;
}
