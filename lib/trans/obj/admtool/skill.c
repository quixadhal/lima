/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** skill.c -- Skill administration
**
** 961213, Deathblade: created.
*/

#include <mudlib.h>

void std_handler(string str);
void input_one_arg(string arg_prompt, function fp, string arg);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_SKILL     "(AdmTool:skill) [larmq?] > "

private nomask void write_skill_menu()
{
    write("Administration Tool: skill administration\n"
	  "\n"
	  "    l        - list skills\n"
	  "    a [name] - add a skill\n"
	  "    r [name] - remove a skill\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void list_skills()
{
    string * skills = sort_array(SKILL_D->query_skills(), 1);
    string * result;

    if ( sizeof(skills) == 0 )
    {
	write("No skills are defined yet.\n");
	return;
    }

    result = ({ sprintf("%-20s %s", "Skill", "Internal name"),
		repeat_string("-", 40) }) +
	map(skills, function (string name)
	    {
		string * parts = explode(name, "/");
		return sprintf("%-20s %s",
			       repeat_string("   ",
					     sizeof(parts)-1) + parts[<1],
			       name);
	    }
	    );

    more(result);
}

private nomask void rcv_add_skill(string skill_name)
{
    string * result;

    skill_name = lower_case(trim_spaces(skill_name));
    if ( skill_name == "" )
	return;

    result = SKILL_D->register_skill(skill_name);
    if ( !result )
	write("ERROR: skill already exists.\n");
    else
	printf("Added: %s.\n", format_list(result));
}

private nomask void rcv_remove_skill(string skill_name)
{
    string * result;

    skill_name = lower_case(trim_spaces(skill_name));

    result = SKILL_D->remove_skill(skill_name);
    if ( !result )
	write("ERROR: no such skill exists.\n");
    else
	printf("Removed: %s.\n", format_list(result));
}

private nomask void receive_skill_input(string str)
{
    string arg;

    sscanf(str, "%s %s", str, arg);

    switch ( str )
    {
    case "l":
	list_skills();
	break;

    case "a":
	input_one_arg("New skill name? ", (: rcv_add_skill :), arg);
	break;

    case "r":
	input_one_arg("Remove which skill? ", (: rcv_remove_skill :), arg);
	break;

    case "?":
	write_skill_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_skill_menu()
{
    if( !check_privilege("Mudlib:daemons") )
    {
	write("Sorry... Mudlib:daemons priv-holders only.\n");
	return;
    }
    modal_func((: receive_skill_input :), PROMPT_SKILL);
    write_skill_menu();
}
