/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** helpsys.c
**
** Standard help system object.  Interfaces with the Help Daemon to
** provide a complete help system to the user.  This object also acts
** as a modal input object via the standard input system functions.
**
** 26-OCT-94.  Created.  Deathblade.
** 14-APR-95   Wirehead added memory for old topcs vs. new ones.
*/

#include <mudlib.h>
#include <daemons.h>

inherit DAEMON;

inherit M_INPUT;


private static string * topic_files;
private static string *	lines;
private static int	cur_line;
private static mapping	directives;

/* hack. */
private static int	i;

nomask void display_topics(mixed);
private nomask void receive_choice(string arg);
private nomask void receive_more(string arg);
private nomask void receive_topic(string arg);

private nomask string query_prompt()
{
    if ( topic_files )
    {
	return sprintf("\nWhich topic to display? [1-%d,?]: ",
		       sizeof(topic_files));
    }

    if ( cur_line )
    {
	return sprintf("--More--(%d%%) [?]: ", 100 * cur_line / sizeof(lines));
    }

    return "\nHelp topic? [?]: ";
}

private nomask void quit_help()
{
    modal_pop();
    destruct(this_object());
}

private nomask int f_parse(string s)
{
    string * x;

    if ( s[0..1] != "$$" )
	return 1;

    if ( s[2] == '#' )
	return 0;

    if ( member_array(':', s) == -1 )
    {
	directives[trim_spaces(s[2..])] = 1;
    }
    else
    {
	x = explode(s[2..], ":");
	directives[trim_spaces(x[0])] = trim_spaces(x[1]);
    }
    
    return 0;
}

private nomask void parse_file(string fname)
{
    cur_line = 0;
    directives = ([ ]);
    
    if( file_size( fname ) == -1 )
    {
    lines = ({ "This helpfile no longer exists.", "Its reference will vanish next time help_d is updated" });

        return;
    }
    lines = explode(read_file(fname), "\n");
    lines = filter_array(lines, (: f_parse :));

    if ( directives["see"] )
    {
	/* ### make this a bit more "in-your-face" ?? */
	lines += ({ "", "See also: " + directives["see"] });
    }
}

private nomask void print_lines()
{
    int count;

    count = this_body()->query_shell_ob()->get_variable("MORE") || 20;
      if(!intp(count)) count =to_int(count);  
    if ( sizeof(lines) - cur_line < count )
	count = sizeof(lines) - cur_line;

    write(implode(lines[cur_line .. (cur_line + count - 1)], "\n") + "\n");

    if ( (cur_line += count) == sizeof(lines) )
    {
	cur_line = 0;
    }
}

/*
** present_topic()
**
** Present a topic to the user
*/
private nomask void present_topic(string fname)
{
    parse_file(fname);

    write("\n");
    print_lines();

    /*
    ** The current receive function is unknown on entry to this function.
    ** Make sure it is set correctly.  cur_line will be zero if print_lines()
    ** ends up printing everything.
    */
    modal_func(cur_line ? (: receive_more :) : (: receive_topic :));
}

private nomask string format_choice(string choice)
{
    return sprintf("%2d. %s", ++i, choice);
}

private nomask void lookup_topic(string topic)
{
    string * files;

    files = HELP_D->find_topic(topic);
    if ( sizeof(files) == 0 )
    {
	write("\nSorry, there is no help on that topic (try: topics)\n\n");
	unguarded(1, (: write_file, "/log/HELP_MISS",
		      sprintf("%s: %s\n", this_user()->query_real_name(),
			      topic) :) );
    }
    else if ( sizeof(files) == 1 )
    {
	present_topic(files[0]);
    }
    else
    {
	/*
	** ### need to devise a better scheme for presenting these.
	** ### preferably, we can use the topic's "parent".  if one
	** ### or more topics don't have parents, though... ??
	** ### not to mention that we don't have their parents until
	** ### we read the files.
	*/
	
	/* ack. must use a global to get the index to work */
	i = 0;

	write("There are multiple help files for \"" + topic + "\"\n" +
	      "Please choose one:\n\n" +
	      implode(map_array(files, (: format_choice :)),
		      "\n") + "\n");
	      
	topic_files = files;
	modal_func((: receive_choice :));
    }
}

private nomask void receive_choice(mixed arg)
{
    if(arg == -1)
      destruct(this_object());

    if ( arg )
	arg = trim_spaces(arg);

    if ( !arg || arg == "" || arg == "q" )
    {
	quit_help();
    }
    else if ( arg == "?" )
    {
	write("\nThe following commands are available:\n\n" +
	      "             q : quit using the help system.\n" +
	      "             ? : this help.\n\n" +
	      "  <topic name> : will display help for the new topic.\n\n"
	      );
    }
    else if ( to_int(arg) > 0 )
    {
	int which;

	which = to_int(arg) - 1;
	if ( which >= sizeof(topic_files) )
	{
	    printf("\nPlease type a number between 1 and %d.\n",
		   sizeof(topic_files));
	}
	else
	{
	    present_topic(topic_files[which]);
	    topic_files = 0;
	}
    }
    else
    {
	topic_files = 0;
	lookup_topic(arg);
    }
}

private nomask void receive_more(mixed arg)
{
    if (arg == -1)
      destruct(this_object());
    if ( arg )
	arg = trim_spaces(arg);

    if ( arg == "q" )
    {
	quit_help();
    }
    else if ( arg == "?" )
    {
	write("\nThe following commands are available:\n\n" +
	      "             q : quit using the help system.\n" +
	      "             ? : this help.\n\n" +
	      "      <return> : will display more of this help topic.\n" +
	      "  <topic name> : will display help for the new topic.\n\n"
	      );
    }
//    else if ( arg == "topics" )
//    {
//	display_topics();
//    }
    else if ( !arg || arg == "" )
    {
	print_lines();
	if ( cur_line == 0 )
	{
	    modal_func((: receive_topic :));
	}
    }
    else
    {
	/*
	** We will ignore the lookup and continue the "more" process or
	** a topic will be found and we'll switch to that one.
	*/
	lookup_topic(arg);
    }
}

private nomask void receive_topic(mixed arg)
{
    if (arg == -1)
      destruct(this_object());

    if ( arg )
	arg = trim_spaces(arg);

    if ( !arg || arg == "" || arg == "q" )
    {
	quit_help();
	return;
    }
    if ( arg == "?" )
	arg = "help";

    lookup_topic(arg);
}

nomask void begin_help(string topic)
{
    if ( !topic || topic == "" )
	topic = "topics";

    modal_push((: receive_topic :), (: query_prompt :));
    lookup_topic(topic);
}



// display_topic_columns and display_topics by Wirehead

private nomask void display_topic_columns(string *files, string header)
{
    string name;
    int i;
    int n = sizeof(files);

    files = map_array(files, (: explode($1, "/")[<1] :) );
    write(header + "\n\n");
    for (i = 0; i < (n/2) + 1; i++) {
	if ((i == n/2) && (n % 2)) {
	    printf("\t%-20s\n", files[i]);
	    break;
	}
	printf("\t%-20s%-20s\n", files[i], files[i + n/2]);
    }
}

// uses this_body()->query_help_topic() to determine what has been
// read recently.
nomask void display_topics(string *arr)
{
    string *new_stuff, *old_stuff;

    old_stuff = filter_array(arr,
			     (: stat($1)[1] <= $(this_body())->query_help_topic($1) :));

    new_stuff = arr - old_stuff;
    if (sizeof(new_stuff)) display_topic_columns(new_stuff, "Unread topics:");
    if (sizeof(old_stuff)) display_topic_columns(old_stuff, "Topics already read:");
}
