/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** more_ob.c
**
** Standard object for providing "more" type functionality.
**
** Originally written by Justice, I believe.
**
** Converted to the new modal input system by Deathblade on 8/16/94.
** Big rewrite and cleanup.
** Beek added ANSI support
** Rust made it check your shell's MORE variable
** Rust added more_lines(lines, chunksize) for intentionally small more's.
*/

#include <mudlib.h>
#include <more.h>

inherit DAEMON;
inherit M_INPUT;
inherit M_ANSI;

private string current_search;
private string last_search;
private int direction;
private string * file_list;
private int file_index;
private string * lines;
private int line_index;
private int chunk_size;
private function continue_func;

private nomask string query_prompt()
{
    string prompt = "";
    int last = line_index + chunk_size;
    int percent;
    
    if (last > sizeof(lines)) {
	last = sizeof(lines);
	percent = 100;
    } else {
	percent = last*100/sizeof(lines);
    }
    
    if (file_list && file_index < sizeof(file_list) && wizardp(this_user()))
	prompt = "\"" + file_list[file_index] + "\" ";
    prompt += sprintf("(%i-%i %i%%) [h]:", line_index + 1, last, percent);

    return ansi("%^BOLD%^" + prompt + "%^RESET%^");
}

/* returns 1 if no more files are available */
private int next_file()
{
    file_index += direction;
    if ( file_index < 0 || file_index >= sizeof(file_list) )
        return 1;

    if (direction == -1)
	printf("more: going back to file \"%s\"\n", file_list[file_index]);
    else
	printf("more: going on to file \"%s\"\n", file_list[file_index]);
    lines = 0;
    return 0;
}

private void print_help()
{
	write(
	  "More Help:\n\n  Commands are one letter anachronyms as listed below\n\n"+
	  "    a : Print current page again.\n"+
	  "    / : /<string> starts foward search for string.\n"+
	  "  h,? : This help, OR ?<string> to start backward search for string.\n"+
	  "    d : Toggle scanning direction.\n"+
	  "        Changes which direction you page through the file(s).\n"+
	  "    b : goto the beginning of the file.\n"+
	  "    e : goto the end of the file.\n"+
	  "    n : Next file in scan direction if any.\n"+
	  "    q : quit\n"+
	  "    s : Set or toggle off search string.\n"+
	  "        s <string> where <string> is what you're looking for.\n"+
	  "        Searches are done in the scan direction and page displayed\n"+
	  "          is the one directly following any match made.\n"+
	  "        Until you toggle s off, any scan will search for a match.\n"+
	  "        s by itself turns off the search parameter\n\n"+
	  "    enter or anything else scans the next page.\n");
}

private void finish()
{
    modal_pop();

    if ( continue_func )
	evaluate(continue_func);

    destruct(this_object());
}

private nomask void do_more(mixed arg) {
    int x;
    string tmp;

    if (arg == -1)
      destruct(this_object());
    if (arg)
	switch(arg[0]) {
	case '?':
	    current_search = arg[1..];
	    
	    if (current_search != "") {
		direction = -1;
		last_search = current_search;
		break;
	    }
	    if (last_search) {
		current_search = last_search;
		direction = -1;
		line_index += chunk_size * direction;
		if (line_index < 0 || line_index >= sizeof(lines)) {
                 if ( next_file()) {
			finish();
			return;
		    }
		}
		break;
	    }
	    write("more: illegal syntax, type \"h\" for help.\n");
	    return;
	case 'h':
	    print_help();
	    return;
	case 's':                           // Set search string
	    if (arg != "s") {
		current_search = arg[1..];
		write("more: search set to \"" + current_search + "\"\n");
	    } else {
		current_search = 0;
		write("more: search off\n");
	    }
	    return;
	case '/':
	    current_search = arg[1..];
	    if (current_search != "") {
		direction = 1;
		last_search = current_search;
		break;
	    }
	    if (last_search) {
		current_search = last_search;
		direction = 1;
		line_index += chunk_size * direction;
		if (line_index < 0 || line_index >= sizeof(lines)) {
		    if ( next_file() ) {
			finish();
			return;
		    }
		}
		break;
	    }
	    write("more: illegal syntax, type \"h\" for help.\n");
	    return;
	case 'd':                           // Toggle Direction
	    direction = -direction;
	    write("more: now scanning " +
		  (direction == 1 ? "forward" : "backward") + "\n");
	    return;
	case 'n':                           // Next file if any
	    if (sizeof(file_list) > 1)
		file_index += direction;
             if (!sizeof(file_list))
            {
                finish();
                return;
            }
	    if (file_index < 0 || file_index >= sizeof(file_list)) {
		write("more: no more files " + (direction == 1 ? "after" : "preceding") +
                          "\"" + file_list[file_index - 1] + "\"\n");
		file_index -= direction;
		return;
	    }
	    else
		lines = 0;
	    break;
	case 'q':
	    finish();
	    return;
	case 'b':
	    line_index = 0;
	    break;
	case 'e':
	    line_index = (sizeof(lines) - chunk_size >= 0 ?
			  sizeof(lines) - chunk_size : 0);
	    break;
	case 'a':
	    break;
	default:                            // Next chunk
	    if (last_search)
		current_search = 0;
	    line_index += chunk_size * direction;
	    if (line_index < 0 || line_index >= sizeof(lines)) {
		if ( next_file() )
		    {
			finish();
			return;
		    }
	    }
	    break;
	}
    while(1) {
	if (!lines) {
            string contents;
	    
	    if (!file_list || !sizeof(file_list))
		break;
	    if(wizardp(this_user()))
		write("filename: "+file_list[file_index]+"\n");
	    if (file_size(file_list[file_index]) == -1) {
		write("more: no such file \"" + file_list[file_index] +
		  "\"\n");
                if ( next_file() )
                    break;
		continue;
	    }
            contents = read_file(file_list[file_index]);
            if ( !contents || !sizeof(lines = explode(contents, "\n")) )
            {
		write("more: file \"" + file_list[file_index] +
		  "\" contains nothing\n");
                if ( next_file() )
                    break;
		continue;
	    }
	    if (direction == -1)
		line_index = (sizeof(lines) - chunk_size >= 0 ?
		  sizeof(lines) - chunk_size : 0);
	    else
		line_index = 0;
	}
	if (current_search) {
	    for(;line_index < sizeof(lines) && line_index >= 0;
	      line_index += direction)
		if (regexp(lines[line_index], current_search))
		    break;
	    if (line_index < 0 || line_index >= sizeof(lines)) {
		write("more: \"" + current_search + "\" not found in \"" +
		  file_list[file_index] + "\"\n");
                if ( next_file() )
                {
                    /* oops. not found. back up and get some input. */
                    file_index -= direction;
                    return;
                }

                /* refill the list of lines */
                continue;
	    }
	}
	for(x = line_index;x < sizeof(lines) && x < line_index + chunk_size; x++)
	    write(lines[x] + "\n");
	if (sizeof(lines) >= chunk_size || (file_list && sizeof(file_list) > 1))
	  {
	    /* return to prompt about more lines/next file */
	    return;
	  }

	/* break: we're done, so finish up */
	break;
    }

    finish();
    return;
}

void create(int kind, mixed arg, int c, function continuation) {
    ::create();

    switch (kind) {
    case 0: // blueprint
	return;
    case MORE_FILES:
	file_list = arg;
	file_index = 0;
	break;
    case MORE_LINES:
	lines = arg;
	break;
    default:
	error("Bad argument 2 to new(MORE_OB, ...)\n");
    }
    direction = 1;
    chunk_size = c;
    continue_func = continuation;
    modal_push((: do_more :), (: query_prompt :));
    do_more(0);
}
