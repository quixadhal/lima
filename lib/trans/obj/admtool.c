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
#include <daemons.h>

inherit M_ACCESS;
inherit M_INPUT;

inherit __DIR__ "admtool/security";
inherit __DIR__ "admtool/domain";
inherit __DIR__ "admtool/money";
inherit __DIR__ "admtool/quest";
inherit __DIR__ "admtool/guild";
inherit __DIR__ "admtool/skill";
inherit __DIR__ "admtool/group";
inherit __DIR__ "admtool/misc";

inherit __DIR__ "admtool/user";
inherit __DIR__ "admtool/banish";
inherit __DIR__ "admtool/i3chan";
inherit __DIR__ "admtool/news";
inherit __DIR__ "admtool/alias";
inherit __DIR__ "admtool/colour";

#define PROMPT_MAIN	"(AdmTool:main) [sdcQGSgMubinaq?] > "

private nomask void receive_main_input(string str);
private nomask void write_main_menu();

private static mapping menu_options = ([
    "s" : (: begin_security_menu :),
    "d" : (: begin_domain_menu :),
    "c" : (: begin_money_menu :),
    "Q" : (: begin_quest_menu :),
    "G" : (: begin_guild_menu :),
    "S" : (: begin_skill_menu :),
    "g" : (: begin_group_menu :),
#ifdef CONFIGURABLE_COLOUR
    "C" : (: begin_colour_menu :),
#endif
    "M" : (: begin_misc_menu :),

    "u" : (: begin_user_menu :),
    "b" : (: begin_banish_menu :),
    "i" : (: begin_i3chan_menu :),
    "n" : (: begin_news_menu :),
    "a" : (: begin_alias_menu :),

    "?" : (: write_main_menu :),
    ]);


private nomask void write_main_menu()
{
    write("\n"
	  "--- Administration Tool ---\n"
	  "\n"
	  "    s - security\n"
	  "    d - domains\n"
	  "    c - currencies\n"	  
	  "    Q - quests\n"
	  "    G - guilds\n"
	  "    S - skills\n"
	  "    g - (mail) groups\n"
#ifdef CONFIGURABLE_COLOUR
	  "    C - configurable colours\n"
#endif
	  "    M - miscellaneous\n"
	  "\n"
	  "    u - users                   [admin]\n"
	  "    b - site/name banishment    [Mudlib:daemons]\n"
	  "    i - Intermud channels       [Mudlib:daemons]\n"
	  "    n - news groups             [Mudlib:daemons]\n"
	  "    a - global aliases          [Mudlib:daemons]\n"
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
    function func = menu_options[str];

    if ( func )
	evaluate(func);
    else
	switch ( str )
	{
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
    if ( clonep() )
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
    input_one_arg(arg_prompt, fp, arg);
}
static nomask void do_two_args(string arg1_prompt,
			       string arg2_prompt,
			       function fp,
			       string arg)
{
    input_two_args(arg1_prompt, arg2_prompt, fp, arg);
}
static nomask void do_three_args(string arg1_prompt,
				 string arg2_prompt,
				 string arg3_prompt,
				 function fp,
				 string arg)
{
    input_three_args(arg1_prompt, arg2_prompt, arg3_prompt, fp, arg);
}

static nomask int write_error(string err)
{
    if ( err )
    {
	write("Error: " + err + ".\n");
	return 1;
    }
}
