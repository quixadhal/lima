/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;
inherit M_ANSI;

void do_print(string which) {
    string array colours;
    string array illegal;
    mapping null = ANSI_D->query_translations()[1];
    int longest;
    
    if (which)
	colours = ({ which });
    else
	colours = this_user()->query_colours();

    if (!colours) {
	write("No colours set.\n");
	return;
    }

    illegal = filter(colours, (: $(null)[$1] == 0 :));
    colours -= illegal;
    
    foreach (string item in colours)
	if (strlen(item) > longest)
	    longest = strlen(item);

    illegal = map(illegal, (: lower_case :));
    if (sizeof(illegal))
	write("Unsupported settings (use 'colours remove whatever'): " + implode(illegal, "\n") + "\n\n");

    colours = map(sort_array(colours, 1), function(string item, int longest) 
		  {
		      string what = this_user()->query_colour(item);
		      if (what)
			  return sprintf("%-"+longest+"s : %%^%s%%^%s%%^RESET%%^", lower_case(item), upper_case(item), what);
		      else
			  return sprintf("%-"+longest+"s : (not set)", item);
		  },
		      longest);
    write(colour_table(colours, this_user()->query_screen_width()) + "\n");
}

nomask private void main(string str) {
    string which, what;
    
#ifndef CONFIGURABLE_COLOUR
    write("Support not enabled.\n");
    return;
#endif

    if (!str) {
	do_print(0);
	return;
    }
    
    if (sscanf(str, "%s %s", which, what) != 2) {
	do_print(str);
	return;
    }

    if (which == "remove") {
	this_user()->remove_colour(what);
	printf("Setting for '%s' removed.\n", what);
	return;
    }
    
    this_user()->set_colour(which, what);
    printf("'%s' set to '%%^%s%%^%s%%^RESET%%^'.\n",
	   which, upper_case(which), what);
}

