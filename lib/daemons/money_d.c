/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** money_d.c -- money daemon
**
** This daemon manages the legal types of money within the game and their
** exchange rates.
**
** Objects have an inherent "value".  This is then translated into a
** particular currency via that currency's value -> actual exchange rate.
**
** Created Wed Jul  3 20:28:42 MET DST 1996 <Valentino>
**
** 961209, Deathblade: Revised extensively and installed into Lima.
**                     Kept Valentino's per-area currencies, but tossed
**                        the materials concept.
*/

inherit M_DAEMON_DATA;

/*
** Mapping of currency names to unit values.  A given currency is worth
** this many "units," which is used for conversion purposes.
*/
private mapping rates = ([ ]);

/*
** Mapping of areas to a list of valid currency types
*/
private mapping areas = ([ ]);

//:FUNCTION query_exchange_rate
//Returns the exchange rate to map from an object's inherent value to its
//actual value in a particular currency.  The value returned is the number
//of units this currency is worth.
nomask int query_exchange_rate(string type)
{
    type = lower_case(trim_spaces(type));
    return rates[type];
}

//:FUNCTION query_currency_types
//Returns the names of the currencies that are valid within the mud
nomask string *query_currency_types()
{
    return keys(rates);
}

nomask void set_exchange_rate(string type, int rate)
{
    if ( !check_privilege("Mudlib:daemons") )
	error("illegal attempt to adjust exchange rate.\n");

    type = lower_case(trim_spaces(type));

    if ( !rate )
	map_delete(rates, type);
    else
	rates[type] = rate;
    save_me();
}

//:FUNCTION query_currency_areas
//Returns the areas with different currencies
nomask string *query_currency_areas()
{
    return keys(areas);
}

//:FUNCTION query_area_currencies
//Returns the currencies that an area uses.
nomask string *query_area_currencies(string area)
{
    area = lower_case(trim_spaces(area));

    return copy(areas[area]);
}

//:FUNCTION add_area_currency
//Add a currency type to an area
nomask void add_area_currency(string area, string type)
{
    area = lower_case(trim_spaces(area));
    type = lower_case(trim_spaces(type));

    if ( !rates[type] )
	error("unknown currency type");

    if ( !areas[area] )
	areas[area] = ({ type });
    else
	areas[area] += ({ type });

    save_me();
}

//:FUNCTION remove_area_currency
//Remove a currency type from an area
nomask void remove_area_currency(string area, string type)
{
    area = lower_case(trim_spaces(area));
    type = lower_case(trim_spaces(type));

    if ( !rates[type] )
	error("unknown currency type");
/* If the currency type doesn't exist, wht's it doing in the area?
 * Furthermore, why can't we remove it  -- Tigran
    if ( !areas[area] )
	error("unknown area");
*/

    areas[area] -= ({ type });
    if ( !sizeof(areas[area]) )
	map_delete(areas, area);

    save_me();
}

void clean_up()
{
    destruct();
}
