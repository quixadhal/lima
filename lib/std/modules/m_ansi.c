/* Do not remove the headers from this file! see /USAGE for more info. */

//:FUNCTION i_use_ansi
// Returns TRUE if the current user (not the object receiving the message!)
// has ansi turned on.
nomask int i_use_ansi()
{
    return get_user_variable("ansi") != 0;
}

string ansi(string str) {
    if ( i_use_ansi() )
	return terminal_colour(str, ANSI_D->query_translations()[0]);
    else
	return terminal_colour(str, ANSI_D->query_translations()[1]);
}

//:FUNCTION colour_strlen
//colour_strlen(s) gives the length of the visible portion of s.  Colour
//codes (e.g. %^GREEN%^) are ignored.
int colour_strlen(string str) {
    return strlen(terminal_colour(str, ANSI_D->query_translations()[1]));
}

string colour_center(string str) {
    int len = colour_strlen(str);
    
    return repeat_string(" ", (this_user()->query_screen_width() - len)/2) + str;
}

//:FUNCTION colour_truncate
//colour_truncate(str, len) returns a string which consists of len or
//fewer visible characters, not counting colour codes.
string colour_truncate(string str, int len) {
    string result;
    int idx;

    result = terminal_colour(str, ANSI_D->query_translations()[2], len);
    
    if ((idx = member_array('\n', result)) == -1)
	return result;
    
    return result[0..idx-1];
}

varargs string colour_table(array items, int width, int num_horiz) {
    int max_len = 0;
    int n = sizeof(items);
    int size_horiz;
    int num_vert;
    int array lens = allocate(n);
    array lists;
    int i;
    
    for (i = 0; i < sizeof(items); i++) {
	lens[i] = colour_strlen(items[i]);
	if (lens[i] > max_len)
	    max_len = colour_strlen(items[i]);
    }

    if (num_horiz == 0) {
	num_horiz = width / (max_len + 2);
	
	if (num_horiz == 0) num_horiz = 1;
	/* This moves some entries from the right side
	 * of the table to fill out the last line,
	 * which makes the table look a bit nicer.
	 * E.g.
	 * (n=13,p=6)      (l=3,p=5)
	 * X X X X X X     X X X X X
	 * X X X X X X  -> X X X X X
	 * X               X X X X
	 *
	 */
	num_vert = (n-1)/num_horiz + 1;
	if (n > num_horiz && n % num_horiz)
	    num_horiz -= (num_horiz - n % num_horiz) / num_vert;
    } else {
	num_vert = (n-1)/num_horiz + 1;
    }

    size_horiz = width / num_horiz;

    if (n < num_horiz)
	num_horiz = n;
    
    lists = map(allocate(num_vert), (: allocate($(num_horiz)) :));
    
    for (i = 0; i < sizeof(items); i++) {
	string tmp;
	
	if (lens[i] > size_horiz) {
	    tmp = colour_truncate(items[i], size_horiz);
	} else {
	    tmp = items[i] + repeat_string(" ", size_horiz - lens[i]);
	}
	lists[i%num_vert][i/num_vert] = tmp;
    }

    return implode(map(lists, (: implode($1, "") :)), "\n");
}
