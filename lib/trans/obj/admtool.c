/* Do not remove the headers from this file! see /USAGE for more info. */

/*
**
** Provides various adminstration facilities for admins, domain lords, and
** wizards.
**
** 07-Jun-95.  Deathblade.  Created.
*/

#include <mudlib.h>
#include <commands.h>
#include <security.h>

inherit M_ACCESS;
inherit M_INPUT;

inherit __DIR__ "admtool/user";
inherit __DIR__ "admtool/i3chan";
inherit __DIR__ "admtool/security";
inherit __DIR__ "admtool/domain";
inherit __DIR__ "admtool/news";
inherit __DIR__ "admtool/alias";
inherit __DIR__ "admtool/quest";
inherit __DIR__ "admtool/banish";
inherit __DIR__ "admtool/group";

#define PROMPT_MAIN	"(AdmTool:main) [usdQbinagq?] > "

private nomask void receive_main_input(string str);


private nomask void write_main_menu()
{
    write("Administration Tool\n"
	  "\n"
	  "    u - user adminstration      [admin]\n"
	  "    s - security adminstration\n"
	  "    d - domain adminstration\n"
	  "    Q - quest administration\n"
	  "    b - site/name banishment    [admin]\n"
	  "    i - Intermud channel admin  [admin]\n"
	  "    n - news administration     [admin]\n"
	  "    a - alias administration    [admin]\n"
	  "    g - group administration\n"
	  "\n"
	  "    q - quit\n"
	  "    ? - help\n"
	  "\n"
	  );
}


static nomask void std_handler(string str)
{
    switch ( str )
    {
    case "":
    case "m":
	modal_func((: receive_main_input :), PROMPT_MAIN);
	write_main_menu();
	break;

    case "q":
	modal_pop();
	destruct();
	break;

    default:
	write("** Unknown option\n");
	break;
    }
}


private nomask void receive_main_input(string str)
{
    switch ( str )
    {
    case "u":
	begin_user_menu();
	break;
	
    case "s":
	begin_security_menu();
	break;
	
    case "d":
	begin_domain_menu();
	break;

    case "Q":
        begin_quest_menu();
	break;
	
    case "b":
	if ( !check_privilege(1) )
	{
	    write("Sorry... admin only.\n");
	    return;
	}
	begin_banish_menu();
	break;

    case "n":
	begin_news_menu();
	break;

    case "a":
	begin_alias_menu();
	break;

    case "i":
	begin_i3chan_menu();
	break;
	
    case "g":
	begin_group_menu();
	break;
	
    case "?":
	write_main_menu();
	break;

    case "":
	str = "q";
	/* FALLTHRU */

    default:
	std_handler(str);
	break;
    }
}

void create()
{
    if (clonep())
    {
        if ( file_name(previous_object()) != CMD_OB_ADMTOOL )
	{
	    write("Illegal attempt to clone ADMTOOL\n");
            destruct(this_object());
            return;
        }
        set_privilege(1);

	write_main_menu();
	modal_push((: receive_main_input :), PROMPT_MAIN);
    }
}

static nomask void do_one_arg(string arg_prompt,
			       function fp,
			       string arg)
{
    if ( !arg )
    {
	write(arg_prompt);
	modal_simple(fp);
	return;
    }

    evaluate(fp, arg);
}

private nomask void rcv_first_of_two(string arg2_prompt,
				     function fp,
				     string arg1)
{
    string arg2;

    if ( arg1 == "" )
    {
	write("Aborted.\n");
	return;
    }

    if ( sscanf(arg1, "%s %s", arg1, arg2) == 2 )
    {
	evaluate(fp, arg1, arg2);
    }
    else
    {
	printf(arg2_prompt, arg1);
	modal_simple((: evaluate, fp, arg1 :));
    }
}

static nomask void do_two_args(string arg1_prompt,
				string arg2_prompt,
				function fp,
				string arg)
{
    if ( arg )
    {
	string arg2;

	if ( sscanf(arg, "%s %s", arg, arg2) == 2 )
	{
	    evaluate(fp, arg, arg2);
	}
	else
	{
	    rcv_first_of_two(arg2_prompt, fp, arg);
	}
    }
    else
    {
	printf(arg1_prompt);
	modal_simple((: rcv_first_of_two, arg2_prompt, fp :));
    }
}

static nomask int write_error(string err)
{
    if ( err )
    {
	write("Error: " + err + ".\n");
	return 1;
    }
}
