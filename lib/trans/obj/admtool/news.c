/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** news.c -- News administration
**
** 950715, Rust: created
*/

#include <mudlib.h>

void std_handler(string str);
void do_one_arg(string arg_prompt, function fp, string arg);
varargs void modal_func(function input_func, mixed prompt_func, int secure);

#define PROMPT_NEWS     "(AdmTool:news) [larmq?] > "

private nomask void write_news_menu()
{
    write("Administration Tool: news administration\n"
	  "\n"
	  "    l        - list newsgroups\n"
	  "    a [name] - add a newsgroup\n"
	  "    r [name] - remove a newsgroup\n"
	  "\n"
	  "    m        - main menu\n"
	  "    q        - quit\n"
	  "    ?        - help\n"
	  "\n"
	  );
}

private nomask void list_newsgroups()
{
    string* grouplist = NEWS_D->get_groups();

    if ( sizeof(grouplist) == 0 )
    {
	write("    <none>\n");
	return;
    }
    grouplist = sort_array(grouplist, 1);
    more(grouplist);

}

private nomask void rcv_add_newsgroup(string str)
{
    str = lower_case(trim_spaces(str));
    if ( str == "" )
	return;

    if ( member_array(str, NEWS_D->get_groups()) != -1 )
    {
	write("** That group already exists.\n");
	return;
    }
    NEWS_D->add_group(str);
    printf("** Group '%s' added.\n", str);
}

private nomask void rcv_remove_newsgroup(string group_name)
{
    string* grouplist = NEWS_D->get_groups();
    
    if ( !group_name )
    {
	write("** no group name supplied.\n");
	return;
    }

    group_name = lower_case(trim_spaces(group_name));
    if ( member_array(group_name, grouplist) == -1 )
    {
	write("** That newsgroup does not exist.\n");
	return;
    }

    NEWS_D->remove_group(group_name);
    printf("** Group '%s' removed.\n", group_name);
}

private nomask void receive_news_input(string str)
{
    string arg;

    sscanf(str, "%s %s", str, arg);

    switch ( str )
    {
    case "l":
	list_newsgroups();
	break;

    case "a":
	do_one_arg("New newsgroup name? ", (: rcv_add_newsgroup :), arg);
	break;

    case "r":
	do_one_arg("Remove which newsgroup? ",
		   (: rcv_remove_newsgroup :),
		   arg);
	break;

    case "?":
	write_news_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_news_menu()
{
    if( !check_privilege("Mudlib:daemons") )
    {
	write("Sorry... Mudlib:daemons priv-holders only.\n");
	return;
    }
    modal_func((: receive_news_input :), PROMPT_NEWS);
    write_news_menu();
}
