/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <classes.h>

inherit CLASS_ALIAS;

void std_handler(string);
void receive_alias_input(string);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_ALIAS    "(AdmTool:alias) [lLaArRmq?] > "


private nomask void write_alias_menu()
{
    write("Alias Menu\n"
	  "\n"
	  "    l         - list global player aliases\n"
	  "    L         - list global wizard aliases\n"
	  "    a [alias] - add a global player alias\n"
	  "    A [alias] - add a global wizard alias\n"
	  "    r [alias] - remove a player alias\n"
	  "    R [alias] - remove a wizard alias\n"
	  "\n"
	  "    m         - main menu\n"
	  "    q         - quit\n"
	  "    ?         - help\n"
	  "\n"
	  );
}

private nomask void add_alias(string name,
			      string expansion,
			      int dev,
			      string xinfo)
{
    int xverb;

    switch ( xinfo )
    {
    case "y":
	xverb = 1;
	break;
    case "":
    case 0:
    case "n":
	break;
    default:
	write("**Invalid selection.\n");
	return;
    }

    modal_func((:receive_alias_input:), PROMPT_ALIAS);
    ALIAS_D->add_default_alias(name, expansion, xverb, dev);
    write("Done.\n");
}
    
varargs private nomask void get_expansion(string name,
					  int dev,
					  string expansion)
{
    modal_func((: add_alias, name, expansion, dev :),
	       "Xalias?  [yn (default n)] ");
}

private nomask void receive_alias_input(string cmd)
{
    string	input;
    mixed 	alias_info;
    mapping aliases;
    string* xaliases;
    string a;
    class alias val;	
    string output;
    int i;

    sscanf(cmd, "%s %s", cmd, input);

    switch(cmd)
    {
    case "l":
    case "L":
	alias_info = ALIAS_D->query_default_aliases();
	if(cmd == "L")
	{
	    aliases = alias_info[2];
	    xaliases = alias_info[3];
	}
	else {
	    aliases = alias_info[0];
	    xaliases=alias_info[1];
	}

	output = sprintf("Alias:%9sExpansion:%29sDefaults:\n","","");
    output += repeat_string("-", 77) + "\n";

	foreach(a, val in aliases)
	{
	    output += sprintf("%-14s %-38s ", a, val->template);
	    if(sizeof(val->defaults) == 1 && val->defaults[0] == "")
		output += "\n";
	    else
	    {
		output += "$*: " + val->defaults[0] + "\n";
		for(i=1;i<sizeof(val->defaults);i++)
		    output += sprintf("%54s$%d: %s\n","",i,val->defaults[i]);
	    }
	}
	more(output);
	break;

    case "a":
    case "A":
	if(input=="" || !input)
	{
	    write(cmd+" [alias]\n");
	    break;
	}
	if(cmd == "a")
	    modal_func((:get_expansion, input,0:), "Expansion: ");
	else
	    modal_func((:get_expansion, input,1:), "Expansion: ");
	break;

    case "r":	
    case "R":
	if(!input || input == "")
	{
	    write(cmd+" [alias]\n");
	    return;
	}
	if(cmd == "r")
	    alias_info = ALIAS_D->query_default_aliases()[0];
	else
	    alias_info = ALIAS_D->query_default_aliases()[2];
	if(undefinedp(alias_info[input]))
	{
	    if(cmd == "r")
		printf("%s wasn't a player alias.\n", input);
	    else 
		printf("%s wasn't a wizard alias.\n", input);
	    return;
	}
	if(cmd == "r")
	    ALIAS_D->remove_default_alias(input);
	else
	    ALIAS_D->remove_default_alias(input,1);
	write("Done.\n");
	return;

    case "?":
	write_alias_menu();
	break;

    default:
	std_handler(cmd);
	break;
    }
}

static nomask void begin_alias_menu()
{
    if ( !check_privilege("Mudlib:daemons") )
    {
	write("Sorry... Mudlib:daemons priv-holders only.\n");
	return;
    }
    modal_func((: receive_alias_input :), PROMPT_ALIAS);
    write_alias_menu();
}
