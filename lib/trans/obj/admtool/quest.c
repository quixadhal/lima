/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** quest.c -- Quest administration
**
** 951124, Rust: created
*/

#include <mudlib.h>


void std_handler(string str);
void do_one_arg(string arg_prompt, function fp, string arg);
varargs void modal_simple(function input_func, mixed prompt, int secure);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_QUEST     "(AdmTool:quest) [lLarmq?] > "

private nomask void write_quest_menu()
{
    write("Administration Tool: quest administration\n"
	  "\n"
	  "    l        - list all places to get points\n"
	  "    L        - list all quests\n"
	  "    a [name] - add quest points for a goal\n"
	  "    r [name] - remove quest points for a goal\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void list_quests()
{
    more(QUEST_D->quest_dump());
}

private nomask void get_milestone(string quest_name, int value, string file,
				  string s)
{
    if ( !stringp(s) || !strlen(s) )
    {
	s = 0;
    }
    QUEST_D->add_quest(quest_name, value, file, s);

    printf("Quest %s added.\n", quest_name);
}

private nomask void get_master(string quest_name, int value, string master)
{
    string file = evaluate_path(master);

    if ( !is_file(file) && !is_file(file+".c") )
    {
	write("That file doesn't exist.\n");
	return;
    }

    modal_simple((: get_milestone, quest_name, value, file :),
		 "If this is a quest endpoint, give a one line description of the quest for\nplayers, or else hit enter (and nothing else!): ");
	     
}


private nomask void get_value(string quest_name, string pts_as_string)
{
    int value = to_int(pts_as_string);
    printf("Quest %s' value is set at %d points.\n", quest_name, value);
    modal_simple((: get_master, quest_name, value :),
		 "What file can dole out these points? ");
}

private nomask void add_quest(string name)
{
    if ( !stringp(name) || !strlen(name) )
    {
	write("Not a valid quest name.\n");
	return;
    }
    if ( QUEST_D->quest_exists(name) )
    {
	write("That quest already exists.\n");
	return;
    }

    modal_simple((: get_value, name :),
		 "How many points is this quest item worth? ");
}


private nomask void remove_quest(string name)
{
    if ( !QUEST_D->delete_quest(name) )
    {
	write("No such quest.\n");
	return;
    }
    else
    {
	write("Deleted.\n");
    }
}


private nomask void receive_quest_input(string str)
{
    string arg;

    sscanf(str, "%s %s", str, arg);

    switch ( str )
    {
    case "l":
	list_quests();
	break;

    case "L":
        more(QUEST_D->dump_final_goals());
	break;

    case "a":
	do_one_arg("Which quest? ", (: add_quest :), arg);
	break;

    case "r":
	do_one_arg("Which quest? ", (: remove_quest :), arg);
	break;

    case "?":
	write_quest_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_quest_menu()
{
    modal_func((: receive_quest_input :), PROMPT_QUEST);
    write_quest_menu();
}
