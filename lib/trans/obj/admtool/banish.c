/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** news.c -- News administration
**
** 951124, Rust: created
*/

#include <mudlib.h>


void std_handler(string str);
varargs void modal_simple(function input_func, int secure);
varargs void modal_func(function input_func, mixed prompt_func, int secure);
void receive_banish_input(string);

#define PROMPT_BANISH     "(AdmTool:banish)   [LlBbUumq?] > "

private nomask void write_banish_menu()
{
    write("Administration Tool: banish administration\n"
	  "\n"
	  "    L                   - list banished names\n"
	  "    l                   - list banished sites\n"
	  "    B <name> <reason>   - banish a name\n"
	  "    b <name> <reason>   - banish a site\n"
	  "    U <name>            - unbanish a name\n"
	  "    u <name>            - unbanish a site\n"
	  
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void receive_banish_input(mixed s)
{
    string str, name, reason;

    if(!sizeof(s))
    {
        s = "q";
}
    s = explode(s," ");
    str = s[0];
    switch(sizeof(s))
      {
      case 3:
	reason = s[2];
      case 2:
	name = s[1];
      }

    switch ( str )
    {
    case "l":
      more(wrap(implode(BANISH_D->show_banishes()[1],", ")));
	break;

    case "L":
      more(wrap(implode(BANISH_D->show_banishes()[0],", ")));
	break;

    case "B":
        if(!name)
	  {
	    write("B <name> <reason>\n");
	    return;
	  }
        BANISH_D->banish_name(name, reason ? reason : "(None given)");
        write("Done.\n");
	break;

    case "b":
        if(!name)
	  {
	    write("b <site> <reason>\n");
	    return;
	  }
        BANISH_D->banish_site(name, reason ? reason : "(None given)");
        write("Done.\n");
        return;

    case "U":
        BANISH_D->unbanish_name(name);
        write("Done.\n");
	break;

    case "u":
        BANISH_D->unbanish_site(name);
        write("Done.\n");
	break;

    case "?":
	write_banish_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_banish_menu()
{
    modal_func((: receive_banish_input :), PROMPT_BANISH);
    write_banish_menu();
}

