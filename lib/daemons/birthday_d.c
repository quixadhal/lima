/* Do not remove the headers from this file! see /USAGE for more info. */

inherit M_DAEMON_DATA;

mapping birthdays = ([]);
mapping reverse = ([]);

nosave string today;

void refresh() {
    today = ctime(time())[4..9];
}

varargs string array query_birthdays(string when) {
    mixed item;

    refresh();
    item = birthdays[when || today];
    return item ? explode(item, ",") : ({ });
}

varargs mapping query_month(string mo) {
    int i;
    mapping ret = ([]);
    
    if (!mo) mo = today[0..2];
    
    for (i = 1; i <= 10; i++) {
	string tmp = birthdays[mo + "  " + i];
	if (tmp) ret[mo + "  " + i] = tmp;
    }
    for (i = 10; i <= 31; i++) {
	string tmp = birthdays[mo + " " + i];
	if (tmp) ret[mo + " " + i] = tmp;
    }
    return ret;
}


void add_birthday(string name, string when) {
    if (strlen(when) == 5)
        when = when[0..2] + " " + when[3..4];
    if (strlen(when) != 6 || when[3] != ' ')
	error("Bad date format\n");

    switch (when[0..2]) {
    case "Jan": case "Feb": case "Mar": case "Apr": case "May": case "Jun":
    case "Jul": case "Aug": case "Sep": case "Oct": case "Nov": case "Dec":
	break;
    case "jan": case "feb": case "mar": case "apr": case "may": case "jun":
    case "jul": case "aug": case "sep": case "oct": case "nov": case "dec":
	when = capitalize(when);
	break;
    default:
	error("Bad date format\n");
    }
    if (member_array(when[4], " 0123456789") == -1
	|| member_array(when[5], "0123456789") == -1)
	error("Bad date format\n");
    if (when[4] == '0')
	when[4] = ' ';
    if (member_array(',', name) != -1)
	error("Bad name\n");

    if (reverse[name]) {
	mixed tmp = explode(birthdays[reverse[name]], ",");
	tmp -= ({ name });
	birthdays[reverse[name]] = implode(tmp, ",");
    }
    reverse[name] = when;

    if (birthdays[when])
	birthdays[when] += "," + name;
    else
	birthdays[when] = name;
    save_me();
}

void report() {
    string array bdays = query_birthdays();
    
    if (!sizeof(bdays))
	return;
    
    bdays = map(bdays, (: capitalize :));
    
    printf("*** Todays birthdays: %s\n", implode(bdays, ", "));
}

void create() {
    int start = time();
    int end = start + 86400;
    string today = ctime(start)[0..9];

    ::create();

    /* binary search for start of tomorrow */
    while (start != end) {
	int mid = (start + end)/2;
	
	if (ctime(mid)[0..9] == today)
	    start = mid + 1;
	else
	    end = mid;
    }

    refresh();
    call_out((: refresh :), start - time());
}

    
