/* Do not remove the headers from this file! see /USAGE for more info. */

inherit "/obj/admtool/internal/base.c";

nomask string module_name() {
    return "currency";
}

nomask string module_key() {
    return "c";
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

nomask class command_info array module_commands() {
    return ({
	new(class command_info,
	    key : "c",
	    desc : "list currencies",
	    action : (: list_currencies :)),
	new(class command_info,
	    key : "e",
	    proto : "[type] [amount]",
	    desc : "set exchange rate",
	    who : "[Mudlib:daemons]",
	    priv : "Mudlib:daemons",
	    args : ({ "What type? ", "What rate? " }),
	    action : (: set_exchange_rate :)),
	new(class command_info,
	    key : "",
	    desc : "(creates type if needed; set to zero to delete)"),
	new(class command_info),
	new(class command_info,
	    key : "l",
	    proto : "[area]",
	    desc : "list currencies (in an area)",
	    args : ({ "What area to list? [all] " }),
	    action : (: list_currencies_by_area :)),
	new(class command_info,
	    key : "a",
	    proto : "[type] [area]",
	    desc : "add a current type to an area",
	    args : ({ "What area? ", "Add which type? " }),
	    action : (: add_area_currency :)),
	new(class command_info,
	    key : "r",
	    proto : "[type] [area]",
	    desc : "remove a current type from an area",
	    args : ({ "What area? ", "Remove which type? " }),
	    action : (: remove_area_currency :))
    });
}
