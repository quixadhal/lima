/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** newsreader.c
**
** Basic newsreader to interface with the news server.
**
** For admins, it also provides adminstrative functions.
**
** 24-Dec-94. Deathblade. Created.
** 2-Jul-95.  John Viega. added completion and menus for the g command.
*/

#include <mudlib.h>
#include <news.h>


inherit M_INPUT;
inherit M_COMPLETE;

private static string	current_group;

#define TOP_PROMPT	("(...) " + mud_name() + " News [q?lg] > ")

nomask void receive_top_cmd(string cmd);
nomask void receive_grp_cmd(string cmd);
nomask void receive_msg_cmd(string cmd);


private nomask void quit_news()
{
    modal_pop();
    destruct(this_object());
}

private nomask string format_group_line(string group)
{
    int last_id;

    last_id = NEWS_D->get_group_last_id(group);
    return sprintf("  %-40s (%d msgs, %d unread)",
		   group, last_id,
		   last_id - this_body()->get_news_group_id(group) + 1);
}

private nomask void add_new_groups()
{
    string * groups;

    groups = NEWS_D->get_groups() - this_body()->subscribed_groups();
    if ( sizeof(groups) == 0 )
	return;

    /* set each next-id-to-read to 1 */
    map_array(groups, "set_news_group_id", this_body(), 1);

    /*
    ** Note: this should occur _after_ the set_news_group_id so that
    ** the number of unread messages is computed correctly.
    **
    ** ### we should ask the user about these. provide options:
    ** ### [ynaql?] or yes (add), no (skip), all, quit (adding), list,
    ** ### and help
    */
    write("Auto-subscribing to:\n" +
	  implode(map_array(groups, (: format_group_line :)), "\n") + "\n");
}

private nomask string grp_cmd_prompt()
{
    return sprintf("(%s) %d msgs unread  [q?lLmgprnc] > ",
		   current_group,
		   NEWS_D->get_group_last_id(current_group) -
		   this_body()->get_news_group_id(current_group) + 1
		   );
}

private nomask string msg_cmd_prompt()
{
    /*
    ** Use player's next-to-read -1 to indicate the current msg
    */
    return sprintf("(%s) %d of %d  [q?lLmgprfFnc] > ",
		   current_group,
		   this_body()->get_news_group_id(current_group) - 1,
		   NEWS_D->get_group_last_id(current_group)
		   );
}

private nomask void switch_to_top()
{
    modal_func((: receive_top_cmd :), TOP_PROMPT);
}

private nomask void switch_to_group()
{
    modal_func((: receive_grp_cmd :), (: grp_cmd_prompt :));
}

private nomask void switch_to_message()
{
    modal_func((: receive_msg_cmd :), (: msg_cmd_prompt :));
}


private static mixed group_selection_menu_items;
private nomask void menu_select_newsgroup(string num)
{
  int index;
  if(num=="q")
    {
      quit_news();
      return;
    }
  if(!sscanf(num,"%d",index) || index < 1 || index > 
     sizeof(group_selection_menu_items))
    {
      write("Invalid selection.\n");
      return;
    }
  current_group = group_selection_menu_items[index-1];
  group_selection_menu_items = 0;
  switch_to_group();
}
  
private nomask void read_group(string group)
{
  string* matches;
  int i;
/*
    if ( group == "" )
    {
	switch_to_top();
	return;
    }
*/
  matches = complete(group, NEWS_D->get_groups());

  switch(sizeof(matches))
    {
    case 0:
      write("No such group.\n");
      return;
    case 1:
      current_group = matches[0];
      break;
    default:
      write("\nSelect group by number:\n"
	    "----------------------------\n");
      for(i=1; i<=sizeof(matches);i++)
	  printf("%-4d%s\n",i,matches[i-1]);

      group_selection_menu_items = matches;
      modal_func((: menu_select_newsgroup:), "[#q] ");
	  
/*      write("multiple completions.  Need a little menu here, but that's "
	    "not done yet.\n");
*/
      return;
    }

    switch_to_group();
}

nomask void receive_group(mixed group)
{
    if(group == -1)
      destruct(this_object());
    read_group(trim_spaces(group));
}

private nomask int test_for_new(string group)
{
    return ( this_body()->get_news_group_id(group) <=
	    NEWS_D->get_group_last_id(group) );
}

private nomask void display_groups_with_new()
{
    string * groups;

    groups = filter_array(this_body()->subscribed_groups(), (: test_for_new :));

    if ( sizeof(groups) == 0 )
    {
	write("No new news.\n");

	modal_push((: receive_top_cmd :), TOP_PROMPT);
    }
    else
    {
	string * list;

	modal_push((: receive_grp_cmd :), (: grp_cmd_prompt :));

	current_group = groups[0];

	list = ({"Groups with new messages:", ""}) +
	    map_array(groups, (: format_group_line :));

	clone_object(MORE_OB)->more_string(list);
    }
}

private nomask void next_group()
{
    string * groups;

    groups = filter_array(this_body()->subscribed_groups(), (: test_for_new :));

    if ( sizeof(groups) == 0 )
    {
	write("No more groups with new news.\n");

	switch_to_top();
    }
    else
    {
	current_group = groups[0];
	write("Moving to: " + current_group + "\n");

	switch_to_group();
    }
}

private nomask string message_line(int id)
{
    mapping msg;

    msg = NEWS_D->get_message(current_group, id);
    return sprintf("%4d. %-35s [%-10s on %s]",
		   id,
		   msg[MSG_SUBJECT][0..39],
		   msg[MSG_POSTER],
		   ctime(msg[MSG_TIME])[4..9]);
}

private nomask void display_messages(int display_all)
{
    int *	ids;
    string *	lines;

    ids = NEWS_D->get_messages(current_group);

    if ( !display_all )
    {
	int read_id;

	read_id = this_body()->get_news_group_id(current_group);
	ids = filter_array(ids, (: $1 >= $(read_id) :) );
    }

    lines = map_array(sort_array(ids, 1), (: message_line :));
    lines = ({sprintf("Messages on %s are:", current_group)}) + lines + ({""});
    clone_object(MORE_OB)->more_string(lines);
}

private nomask void receive_post_subject(mixed subject)
{
    if (subject == -1)
      destruct(this_object());

    if ( subject == "" )
    {
	write("Post aborted.\n");
	return;
    }
    
    clone_object(EDIT_OB)->edit_text(0, "receive_post_text", subject);
}

nomask void receive_post_text(mixed ctx, string * text)
{
    int id;

    if ( !text )
    {
	write("Post aborted.\n");
	return;
    }
    
    id = NEWS_D->post(current_group, ctx, implode(text, "\n") + "\n");
    write("Posted:  " + message_line(id) + "\n");
}

private nomask void post_message()
{
    write("Subject: ");
    modal_simple((: receive_post_subject :));
}

private nomask void followup_to_message()
{
    clone_object(EDIT_OB)->edit_text(0, "receive_followup_text", 0);
}

private nomask void followup_with_message()
{
    int current_id;
    mapping msg;
    string * lines;

    /*
    ** -1 to get the "current" message.  The player records the _next_
    ** message to read.
    */
    current_id = this_body()->get_news_group_id(current_group) - 1;
    msg = NEWS_D->get_message(current_group, current_id);
    lines = ({ sprintf("On %s %s wrote:",
		       ctime(msg[MSG_TIME]),
		       msg[MSG_POSTER]) });

    lines += map_array(explode(msg[MSG_MESSAGE], "\n"),
		       (: "> " + $1 :) );

    clone_object(EDIT_OB)->edit_text(lines, "receive_followup_text", 0);
}

private nomask int read_next_message()
{
    int current_id;
    mapping msg;
    string post;

    current_id = this_body()->get_news_group_id(current_group);
    msg = NEWS_D->get_message(current_group, current_id);
    if ( !msg )
    {
	write("No more messages.\n");
	switch_to_group();
	return 1;
    }

    /*
    ** Set the next-to-read id.  Always move to the message prompt.
    */
    this_body()->set_news_group_id(current_group, current_id + 1);
    switch_to_message();

    post = sprintf("Time:    %s\n"
		   "Poster:  %s\n"
		   "Subject: %s\n"
		   "\n"
		   "%s",
		   ctime(msg[MSG_TIME]), msg[MSG_POSTER],
		   msg[MSG_SUBJECT], msg[MSG_MESSAGE]);
    clone_object(MORE_OB)->more_string(post);

    return 0;
}

private nomask void global_commands(string cmd)
{
    if ( cmd == "q" )
    {
	quit_news();
    }
    else if ( cmd[0] == 'g' )
    {
	if ( strlen(cmd) > 1 )
	{
	    read_group(trim_spaces(cmd[1..]));
	}
	else
	{
	    modal_func((: receive_group :),
		       "Which group (Or enter for a menu)? ");
	}
    }
    else
    {
	write("Unknown command. Type ? for help.\n");
    }
}

private nomask void group_commands(string cmd)
{
    int id;

    if ( cmd == "l" || cmd == "L" )
    {
	display_messages(cmd == "L");
    }
    else if ( cmd == "m" )
    {
	switch_to_top();
    }
    else if ( cmd == "p" )
    {
	post_message();
    }
    else if ( cmd == "n" )
    {
	next_group();
    }
    else if ( cmd == "c" )
    {
	id = NEWS_D->get_group_last_id(current_group) + 1;
	this_body()->set_news_group_id(current_group, id);
	write("All posts marked as read.\n");
	switch_to_group();
    }
    else if ( cmd == "" )
    {
	if ( read_next_message() )
	{
	    next_group();
	}
    }
    else if ( cmd[0] == 'r' )
    {
	if ( strlen(cmd) > 1 )
	{
	    id = to_int(trim_spaces(cmd[1..]));
	    if ( id < 1 )
		id = 1;
	    else if ( id > NEWS_D->get_group_last_id(current_group) )
		id = NEWS_D->get_group_last_id(current_group);
	    this_body()->set_news_group_id(current_group, id);
	}

	read_next_message();
    }
    else if( id = to_int(trim_spaces(cmd)) )
      {
	if ( id < 1 )
	  id = 1;
	else if ( id > NEWS_D->get_group_last_id(current_group) )
	  id = NEWS_D->get_group_last_id(current_group);
	this_body()->set_news_group_id(current_group, id);
	read_next_message();
      }
    else
    {
	global_commands(cmd);
    }
}

nomask void receive_followup_text(mixed ctx, string * text)
{
    int id;

    if ( !text )
    {
	write("Post aborted.\n");
	return;
    }
    
    /*
    ** -1 to get the "current" message.  The player records the _next_
    ** message to read.
    */
    id = NEWS_D->followup(current_group,
			  this_body()->get_news_group_id(current_group) - 1,
			  implode(text, "\n") + "\n");
    write("Posted:  " + message_line(id) + "\n");
}

nomask void begin_reading(string arg)
{
    this_body()->validate_groups();
    
    add_new_groups();

    display_groups_with_new();
}

nomask void receive_top_cmd(mixed cmd)
{
    if(cmd == -1)
      destruct(this_object());
    cmd = trim_spaces(cmd);
    if ( cmd == "?" )
    {
	write("\n" +
	      wrap("This is the prompt for commands that apply before "
		   "you have picked a newsgroup.  The commands that "
		   "apply are:") +
	      "\n"
	      "  q   quit reading news\n"
	      "  ?   this help\n"
	      "  l   list newsgroups\n"
	      "  g   go to a newsgroup\n"
	      "\n"
	      "Just type a command at the prompt and hit return.\n"
	      );
    }
    else if ( cmd == "l" )
    {
	string * list;

	list = map_array(NEWS_D->get_groups(), (: format_group_line :));
	list = ({ "", "Available groups are:" }) + list;
	clone_object(MORE_OB)->more_string(list);
    }
    else
    {
	global_commands(cmd);
    }
}

nomask void receive_grp_cmd(mixed cmd)
{
    if(cmd == -1)
      destruct(this_object());
    cmd = trim_spaces(cmd);
    if ( cmd == "?" )
    {
	write("\n" +
	      wrap("This is the prompt for commands that apply to the "
		   "current newsgroup you are reading (" + current_group +
		   ").  The commands that apply are:") +
	      "\n"
	      "  q   quit reading news\n"
	      "  ?   this help\n"
	      "  l   list new messages\n"
	      "  L   list all messages\n"
	      "  m   main news menu\n"
	      "  g   go to a newsgroup\n"
	      "  p   post message\n"
	      "  r   read messages\n"
	      "  n   next newsgroup with new news\n"
	      "  c   mark all posts as read (catch up)\n"
	      "\n"
	      "  g group   goes to newsgroup \"group\"\n"
	      "  r number  reads messages starting at \"number\"\n"
	      "  <return>  read messages or go to next newsgroup with new news\n"
	      "\n"
	      "Just type a command at the prompt and hit return.\n"
	      );
    }
    else
    {
	group_commands(cmd);
    }
}

nomask void receive_msg_cmd(mixed cmd)
{
    if(cmd == -1)
      destruct(this_object());
    cmd = trim_spaces(cmd);
    if ( cmd == "?" )
    {
	write("\n" +
	      wrap("This is the prompt for commands that apply to the "
		   "current newsgroup you are reading (" + current_group +
		   ").  The commands that apply are:") +
	      "\n"
	      "  q   quit reading news\n"
	      "  ?   this help\n"
	      "  l   list new messages\n"
	      "  L   list all messages\n"
	      "  m   main news menu\n"
	      "  g   go to a newsgroup\n"
	      "  p   post message\n"
	      "  r   read next message\n"
	      "  f   post a followup to this message\n"
	      "  F   post a followup to this message, quoting it\n"
	      "  n   next newsgroup with new news\n"
	      "  c   mark all posts as read (catch up)\n"
	      "\n"
	      "  g group   goes to newsgroup \"group\"\n"
	      "  r number  reads messages starting at \"number\"\n"
	      "  <return>  read messages or go to next newsgroup with new news\n"
	      "\n"
	      "Just type a command at the prompt and hit return.\n"
	      );
    }
    else if ( cmd == "f" )
    {
	followup_to_message();
    }
    else if ( cmd == "F" )
    {
	followup_with_message();
    }
    else
    {
	group_commands(cmd);
    }
}
