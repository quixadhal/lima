/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** money.c -- Currency administration
**
** 25-Jul-96   Valentino.    Created.
**  6-Aug-96   Valentino.    Generalized and ported to Da Bean.
** 961209, Deathblade:       updated for new MONEY_D
*/

#include <daemons.h>

void std_handler(string str);
varargs void modal_func(function input_func, mixed prompt_func, int secure);
void do_one_arg(string arg_prompt, function fp, string arg);
void do_two_args(string arg1_prompt, string arg2_prompt,
		 function fp, string arg);

#define PROMPT_MONEY     "(AdmTool:currency) [celarmq?] > "

private nomask void write_money_menu()
{
    write("Administration Tool: currency administration\n"
	  "\n"
	  "    c                  - list currencies\n"
	  "    e [type] [amount]  - set exchange rate              [Mudlib:daemons]\n"
	  "                         (creates type if needed; set to zero to delete)\n"
	  "\n"
	  "    l [area]           - list currencies (in an area)\n"
	  "    a [type] [area]    - add a current type to an area\n"
	  "    r [type] [area]    - remove a current type from an area\n"
	  "\n"
	  "    m                  - main menu\n"
	  "    q                  - quit\n"
	  "    ?                  - help\n"
	  "\n"
	  );
}

private nomask void print_currencies(string area, string *types)
{
    int first = 1;
    foreach ( string type in types )
    {
	if ( first )
	{
	    printf("%-20s %-20s %d\n", area, capitalize(type),
		   MONEY_D->query_exchange_rate(type));
	    first = 0;
	}
	else
	{
	    printf("%-20s %-20s %d\n", "", capitalize(type),
		   MONEY_D->query_exchange_rate(type));
	}
    }
    write("\n");
}

private nomask void list_currencies_by_area(string area)
{
    string *types = MONEY_D->query_currency_types();
    string *areas = MONEY_D->query_currency_areas();
    string *avail;

    area = lower_case(trim_spaces(area));
    if ( area == "all" )
	area = "";

    if ( area != "" && (!areas || member_array(area, areas) == -1) )
    {
	printf("Area \"%s\" has no currencies.\n", capitalize(area));
	return;
    }

    printf("%-20s %-20s %s\n%*'-'s\n", "Area", "Currency", "Rate", 73, "");
    if ( area != "" )
    {
	avail = MONEY_D->query_area_currencies(area);

	print_currencies(capitalize(area), avail);
    }
    else
    {
	foreach ( area in areas )
	{
	    avail = MONEY_D->query_area_currencies(area);

	    print_currencies(capitalize(area), avail);

	    /* we want to find unassigned types */
	    types -= avail;
	}

	if ( sizeof(types) )
	{
	    print_currencies("<unassigned>", types);
	}
    }
}

private nomask void list_currencies()
{
    string *types = MONEY_D->query_currency_types();

    if ( sizeof(types) == 0 )
    {
	write("No currencies are defined.\n");
    }
    else
    {
	printf("%-20s %s\n%*'-'s\n", "Currency", "Rate", 73, "");
	foreach ( string type in types )
	{
	    printf("%-20s %d\n", capitalize(type),
		   MONEY_D->query_exchange_rate(type));
	}
    }
}

private nomask void set_exchange_rate(string type, string rate)
{
    int r = to_int(rate);

    type = capitalize(lower_case(trim_spaces(type)));
    MONEY_D->set_exchange_rate(type, r);

    if ( !r )
	printf("Currency type \"%s\" has been deleted.\n", type);
    else
	printf("Exchange rate for \"%s\" set to %d.\n", type, r);
}

private nomask void add_area_currency(string area, string type)
{
    area = capitalize(lower_case(trim_spaces(area)));
    type = capitalize(lower_case(trim_spaces(type)));
    MONEY_D->add_area_currency(area, type);
    printf("Currency type \"%s\" has been added to area \"%s\".\n", type, area);
}

private nomask void remove_area_currency(string area, string type)
{
    area = capitalize(lower_case(trim_spaces(area)));
    type = capitalize(lower_case(trim_spaces(type)));
    MONEY_D->remove_area_currency(area, type);
    printf("Currency type \"%s\" has been removed from area \"%s\".\n", type, area);
}

private nomask void receive_money_input(string str)
{
    string arg;

    sscanf(str, "%s %s", str, arg);

    switch ( str )
    {
    case "c":
	list_currencies();
	break;

    case "e":
	if ( !check_privilege("Mudlib:daemons") )
	{
	    write("Sorry, only admins can change currencies.\n");
	    return;
	}
	do_two_args("What type? ",
		    "What rate? ",
		    (: set_exchange_rate :),
		    arg);
	break;

    case "l":
	do_one_arg("What area to list? [all] ",
		   (: list_currencies_by_area :),
		   arg);
	break;

    case "a":
	do_two_args("What area? ",
		    "Add which type? ",
		    (: add_area_currency :),
		    arg);
	break;

    case "r":
	do_two_args("What area? ",
		    "Remove which type? ",
		    (: remove_area_currency :),
		    arg);
	break;

    case "?":
	write_money_menu();
	break;

    default:
	std_handler(str);
	break;
    }
}

static nomask void begin_money_menu()
{
    modal_func((: receive_money_input :), PROMPT_MONEY);
    write_money_menu();
}
