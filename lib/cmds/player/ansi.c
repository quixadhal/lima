/* Do not remove the headers from this file! see /USAGE for more info. */

//  Yaynu@Lima Bean
#include <mudlib.h>
inherit CMD;
inherit M_ANSI;
 
private void main(string arg)
{
    if ( !arg )
    {
	if ( i_use_ansi() )
	    out("Ansi is "+
		ansi("%^BOLD%^%^MAGENTA%^on.%^RESET%^")+"\n");
	else
	    out("Ansi is off.\n");
	return;
    }

    if ( arg != "on" && arg != "off" )
    {
        out("Usage: ansi <on/off>.\n");
        return;
    }

    if ( arg == "on" )
    {
        this_user()->query_shell_ob()->set_variable("ansi", 1);
        out("Turning ansi on.\n");
    }
    else
    {
	this_user()->query_shell_ob()->unset_variable("ansi");
	out("Turning ansi off.\n");
    }

    this_user()->update_translations();
}                                    

void player_menu_entry(string str)
{
    bare_init();
    main(str);
    done_outputing();
}
