/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** newsreader.c
**
** Basic newsreader to interface with the news server.
**
** 941224, Deathblade: created.
** 950702, Rust: added completion and menus for the g command.
** 950811, Deathblade: convert to use class news_msg.
** Ohara: Lots of misc tweaks, chromefixes, bugfixes, etc. RTFDL ;)
** 971103, Tigran:  Lots of changes.  Proper group subscription, and threading
**                  Functionality added.  
*/


#include <classes.h>
#include <edit.h>

/* 
 * In order to properly convert news from the user to the body, or vice versa,
 * you need to make sure that newsdata.c is inherited both by the body and 
 * the user objects.  It is recommended that its location be where you 
 * intend to leave it (for body in /std/body/ and for user in /secure/user/).
 * And NEWS_DATA_IN_BODY must be defined to have the newsdata in the body 
 * and undefined for the news_data to be in the user. 
 */


inherit M_INPUT;
inherit M_COMPLETE;
inherit M_GRAMMAR;
inherit M_SET;

inherit CLASS_NEWSMSG;


private nosave string   current_group;
private nosave int      current_id;
private nosave int      current_thread;

#define TOP_PROMPT      ("(...) " + mud_name() + " News [q?lg] > ")

#ifdef NEWS_DATA_IN_USER
# define SAVE_OB this_user()
# define OLD_SAVE_OB this_body()
#else
# define SAVE_OB this_body()
# define OLD_SAVE_OB this_user()
#endif

#define STANDARD_WRAP_WIDTH     68
#define FOLLOWUP_WRAP_WIDTH     76

private nomask void display_groups_with_new(string arg);
varargs nomask void begin_reading(string arg);
nomask void receive_top_cmd(string cmd);
nomask void receive_grp_cmd(string cmd);
nomask void receive_msg_cmd(string cmd);
nomask void next_message();

private nomask void quit_news()
{
    /* save the player's news reading state */
    SAVE_OB->save_me();
    modal_pop();
    destruct();
}

private string get_unread_ids(string group)
{
  return set_difference(set_add_range("",1,NEWS_D->get_group_last_id(group) ),
                        SAVE_OB->get_news_id_read(group) );
}

private int get_lowest_unread_id(string group)
{
  int i;
  i=set_min(get_unread_ids(group));
  if(!i)
    return 0;
  /* If the messgae doesn't exist it has either been archived or imporperly 
   * removed.  Mark it read and find something different */
  if(member_array(i,NEWS_D->get_messages(group))==-1)
    {
      SAVE_OB->add_news_id_read(group,i);
      i=get_lowest_unread_id(group);
    }
  return i;
}

private int get_lowest_unread_thread_id(string group)
{
  return NEWS_D->get_thread_id(group,
                               get_lowest_unread_id(group) );
}

private void receive_search(int flag, string str) {
    mixed array results;

    if(!sizeof(str))
      {
        write( "You must specify a search.\n");
        return;
      }
    if ( flag )
        results = NEWS_D->search_for_author(str);
    else
        results = NEWS_D->search_for(str);

    if (!sizeof(results)) {
        write("No matches.\n");
        return;
    }
    more(({ sizeof(results) + " matches:", "" }) + map(results, (: $1[0] + ":" + $1[1] :)));
}

private int count_active_messages(string group)
{
  return sizeof(NEWS_D->get_messages(group,1) );
}

private int count_unread_messages(string group)
{
  return set_count(get_unread_ids(group));
}

private int unread_thread_filter(int thread_id,string group)
{
  if(set_member(get_unread_ids(group),thread_id))
    return 1;
}

private int array unread_thread_ids(string group,int thread)
{
  int array ids;
  ids=filter(NEWS_D->get_thread(group,thread), (: member_set :),get_unread_ids(group));
  return sort_array(ids,1);
}

private nomask string format_group_line(string group)
{
    int last_id;
    int all = count_active_messages(group);

    last_id = NEWS_D->get_group_last_id(group);
    return sprintf("  %-40s (%d %s, %d unread)",
                   group,
                   all,
                   all == 1 ? "message" : "messages",
                   count_unread_messages(group));
}

nomask void rcv_group_answer(string *groups,string input)
{
  switch(input)
    {
    case "y":
      SAVE_OB->subscribe_group(groups[0]);
      break;
    case "n":
      SAVE_OB->unsubscribe_group(groups[0]);
      break;
    case "q":
      return;
    default:
      modal_simple( (:rcv_group_answer,groups:),
                    sprintf("Subscribe to %s [ynq]? ",
                            groups[0]) );
      return;
    }
  groups-=({groups[0]});
  if(sizeof(groups))
    modal_simple( (:rcv_group_answer,groups:),
                  sprintf("Subscribe to %s [ynq]? ",
                          groups[0]) );
  else 
    display_groups_with_new("");
  return;
}

varargs private nomask void add_new_groups(string arg)
{
  string * groups;
  if(groups=SAVE_OB->registered_groups())
    groups=NEWS_D->get_groups()-groups;
  else 
    groups=NEWS_D->get_groups();
  if( !sizeof(groups) )
    {
      display_groups_with_new(arg);
      return;
    }
  modal_simple( (:rcv_group_answer,groups :), sprintf("Subscribe to %s? [ynq] ",
                                                     groups[0]) );
  return;
}

private nomask string grp_cmd_prompt()
{
    int unread_no;

    unread_no = count_unread_messages(current_group);

    return sprintf("(%s:%d) %d %s unread of %d [q?lLmgsSpunc] > ",
                   current_group,
                   current_id,
                   unread_no,
                   unread_no == 1 ? "msg" : "msgs",
                   count_active_messages(current_group)
                   );
}

private nomask string msg_cmd_prompt()
{
  return sprintf("(%s:%O) %d left [q?lLmghsSprRfFuncMD] > ",
		 current_group,
		 current_id,
		 count_unread_messages(current_group)
		 );
}

private nomask void switch_to_top()
{
    modal_push((: receive_top_cmd :), TOP_PROMPT);
}

private nomask void switch_to_group()
{
    modal_func((: receive_grp_cmd :), (: grp_cmd_prompt :));
}

private nomask void switch_to_message()
{
    modal_func((: receive_msg_cmd :), (: msg_cmd_prompt :));
}


private nosave mixed group_selection_menu_items;
private nomask void menu_select_newsgroup(string num)
{
    int index;

    if ( num=="q" )
    {
        write( "Selection aborted.\n");
        if( !current_group) destruct( this_object());
        switch_to_message();
        return;
    }

    if ( !sscanf(num,"%d",index) || index < 1 || index > 
      sizeof(group_selection_menu_items) )
    {
        write("Invalid selection.\n");
        return;
    }

    current_group = group_selection_menu_items[index-1];
    group_selection_menu_items = 0;
    switch_to_group();
}


private nomask void menu_select_change_header( string header )
{
    NEWS_D->change_header( current_group, get_lowest_unread_id(current_group), header );
}


private nomask void menu_select_movegroup(string num)
{
    string to_group;
    int index;
    if( num == "q" )
    {
        write( "Aborting Move.\n" );
        switch_to_message();
        return;
    }
    if( !sscanf( num, "%d", index )|| index < 1 || index >
      sizeof( group_selection_menu_items))
    {
        write( "Invalid selection.\n");
        return;
    }
    to_group = group_selection_menu_items[index - 1];
    group_selection_menu_items = 0;
    NEWS_D->move_post( current_group, current_id, to_group);
    switch_to_message();
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
    switch ( sizeof(matches) )
    {
    case 0:
        write("No such group.\n");
        if(!current_group) destruct( this_object());
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
        return;
    }
    switch_to_group();
}

private nomask void receive_group(mixed group)
{
    if ( group == -1 )
        destruct(this_object());

    read_group(trim_spaces(group));
}

private nomask int test_for_new(string group)
{
    if( count_unread_messages( group )>0) return 1;
    return 0;
}

private nomask void display_groups_with_new(string arg)
{
    string * groups;

    /*
    ** Argument options:
    **   -c   Check news. Display new news and then exit.
    */
    groups = filter_array(SAVE_OB->subscribed_groups(), (: test_for_new :));

    if(!sizeof(groups))
    {
        write("No new news.\n");

        if ( arg == "-c" )
        {
            destruct( this_object());
            return;
        }
        modal_push((: receive_top_cmd :), TOP_PROMPT);
    }
    else
    {
        string * list;

        modal_push((: receive_grp_cmd :), (: grp_cmd_prompt :));

        current_group = groups[0];

        list = ({ "Groups with new messages:", "" }) +
        map_array(groups, (: format_group_line :));

        /* if -c was given, then quit after displaying the new stuff */
        if ( arg == "-c" )
            more(list, 0, (: quit_news :));
        else
            more(list);
    }
}

private nomask void next_group()
{
    string * groups;
    int max_group;
    int i;
    int next_group;
    
    groups = filter_array(SAVE_OB->subscribed_groups(), (: test_for_new :));
    max_group = sizeof( groups ) - 1;
    if( max_group == -1 )
    {
        write("No more groups with new news.\n");
        switch_to_top();
        current_group=0;
        current_thread=0;
        current_id=0;
    }
    else
    {
      for( i = 0; i <= max_group; i++ )
        {
          if( groups[i] == current_group )
            {
              if( i == max_group )
                next_group = 0;
              else
                next_group = i + 1;
              break;
            }
        }
      current_group = groups[next_group];
      write("Moving to: " + current_group + "\n");
      current_thread=0;
      current_id=0;
      switch_to_group();
    }
}

private nomask void next_thread()
{
  if(!current_group||!count_unread_messages(current_group))
    {
      current_thread=0;
      next_group();
      return;
    }
  current_thread=get_lowest_unread_thread_id(current_group);
  next_message();
}

private nomask void next_message()
{
  int array ids;
  if(!sizeof(ids=unread_thread_ids(current_group,current_thread) ) )
    {
      next_thread();
      return;
    }
  current_id=ids[0];
  return;
}
  
private nomask
string format_message_line(int short_fmt, int id)
{
    class news_msg msg;
    string subject;

    msg = NEWS_D->get_message(current_group, id);

    if ( !msg || !msg->body )
        return sprintf(short_fmt ? "%d. %s" : "%4d. %-35s", id, "(removed)");
    else
        subject = msg->subject;

    return sprintf(short_fmt ? "%d. %s  [%s on %s]" :
      "%4d. %-35s [%-10s on %s]",
      id,
      subject[0..34],
      msg->poster,
      intp(msg->time) ? ctime(msg->time)[4..9] : msg->time);
}

private nomask int sort_messages(int first,int second)
{
  int i1=NEWS_D->get_thread_id(current_group,first);
  int i2=NEWS_D->get_thread_id(current_group,second);
  return (i1 < i2) ? -1 : ( ( i1 > i2 ) ? 1 : ( ( first < second ) ? -1 : 1 ) );
}

private nomask void display_messages(int display_all)
{
    int array ids;
    string array lines;
    ids=NEWS_D->get_messages(current_group, 1); 
    if ( !display_all )
      {
	string unread;
	unread=get_unread_ids(current_group);
	ids = filter(ids, (: member_set :), unread);
      }
    ids=sort_array(ids,(:sort_messages:) );
    lines = map_array(ids,
		      (: format_message_line(0, $1) :) );
    lines = ({sprintf("Messages on %s are:", current_group)}) + lines + ({""});
    more(lines);
}

nomask void wrap_post(string * text)
{
    int i = sizeof(text);

    while ( i-- )
    {
        /* only wrap really long lines */
        if ( sizeof(text[i]) > 78 )
        {
            /* wrap to different widths, based on the text */
            int width = text[i][0]=='>' ? FOLLOWUP_WRAP_WIDTH : STANDARD_WRAP_WIDTH;
            if ( sizeof(text[i]) > width )
                text[i] = wrap(text[i], width);
        }
    }
}

private nomask void receive_post_text(string subject, string * text)
{
    int id;

    if ( !text )
    {
        write("Post aborted.\n");
        return;
    }

    wrap_post(text);

    id = NEWS_D->post(current_group, subject, implode(text, "\n") + "\n");
    write("Posted:  " + format_message_line(1, id) + "\n");
}

private nomask void receive_post_subject(mixed subject)
{
    if ( subject == -1 )
        destruct(this_object());

    if ( subject == "" )
    {
        write("Post aborted.\n");
        return;
    }

    new(EDIT_OB, EDIT_TEXT, 0, (: receive_post_text, subject :));
}

private nomask void post_message()
{
    if( NEWS_D->is_write_restricted( current_group ))
    {
        write( "You may not post to " + current_group + ".\n");
        return;
    }
    modal_simple((: receive_post_subject :), "Subject: ");
}

nomask void receive_reply_text( string * text)
{
    class news_msg msg = ((class news_msg)NEWS_D->get_message(current_group,current_id));
    if ( !text )
    {
        write("Post aborted.\n");
        return;
    }

    wrap_post(text);

    this_body()->query_mailer()->send_news_reply(
      "Re: " + msg->subject,
      text,
      lower_case( msg->poster )
    );
}

private nomask void reply_to_message()
{
    class news_msg msg = ((class news_msg)NEWS_D->get_message(current_group,current_id));

    if ( !msg )
    {
        write("You may not reply to that message -- it was removed.\n");
        return;
    }
    new(EDIT_OB, EDIT_TEXT, 0, (: receive_reply_text :));
}

private nomask void reply_with_message()
{
    class news_msg msg = ((class news_msg)NEWS_D->get_message(current_group,current_id));
    string * lines;

    if ( !msg )
    {
        write("You may not reply to that message -- it was removed.\n");
        return;
    }
    lines = ({ sprintf("On %s %s wrote post #%d in %s:",
        intp(msg->time) ? ctime(msg->time) : msg->time,
        msg->poster,
        current_id,
        current_group,
      )
    });
    lines += map_array(explode(msg->body, "\n"), (: "> " + $1 :) );
    new(EDIT_OB, EDIT_TEXT, lines, (: receive_reply_text :));
}

nomask void receive_followup_text(string * text)
{
    int id;

    if ( !text )
    {
        write("Post aborted.\n");
        return;
    }

    wrap_post(text);

    /*
    ** -1 to get the "current" message.  The player records the _next_
    ** message to read.
    */
    id = NEWS_D->followup(current_group,
			  current_id,
			  implode(text, "\n") + "\n");
    write("Posted:  " + format_message_line(1, id) + "\n");
}

private nomask void followup_to_message()
{
    class news_msg msg = ((class news_msg)NEWS_D->get_message(current_group,current_id));

    if ( !msg->body )
    {
        write("You may not followup to that message -- it was removed.\n");
        return;
    }
    if( NEWS_D->is_write_restricted( current_group ))
    {
        write( "You may not post to " + current_group + ".\n");
        return;
    }
    new(EDIT_OB, EDIT_TEXT, 0, (: receive_followup_text :));
}

private nomask void followup_with_message()
{
    class news_msg msg = ((class news_msg)NEWS_D->get_message(current_group,current_id));
    string * lines;

    if ( !msg->body )
    {
        write("You may not followup to that message -- it was removed.\n");
        return;
    }
    if( NEWS_D->is_write_restricted( current_group ))
    {
        write( "You may not post to " + current_group + ".\n");
        return;
    }
    lines = ({ sprintf("On %s %s wrote post #%d:",
        intp(msg->time) ? ctime(msg->time) : msg->time,
        msg->poster, current_id) });

    lines += map_array(explode(msg->body, "\n"), (: "> " + $1 :) );

    new(EDIT_OB, EDIT_TEXT, lines, (: receive_followup_text :));
}

private nomask void read_message(string group,int id)
{
    class news_msg msg;
    string post;
    msg = NEWS_D->get_message(current_group,id);
    /*
    ** Set the next-to-read id.  Always move to the message prompt.
    */
    SAVE_OB->add_news_id_read(current_group,id);
    switch_to_message();
    if ( msg )
    {
        post = sprintf("Time:    %%^NEWS_TIME%%^%-40s%%^RESET%%^Post-id: %d (%d Last)\n"
                       "Poster:  %%^NEWS_POSTER%%^%s%%^RESET%%^\n"
                       "Subject: %%^NEWS_SUBJECT%%^%s%%^RESET%%^\n"
                       "\n"
                       "%s",
                       intp(msg->time) ? ctime(msg->time) : msg->time,
                       id,
                       NEWS_D->get_group_last_id(current_group),
                       msg->poster,
                       msg->subject,
                       msg->body ? msg->body : "*** REMOVED ***");
    }
    else
    {
      write("No such message\n");
      return;
    }
    more(post);
}

private nomask void toggle_subscription(string group)
{
  if(member_array(group,NEWS_D->get_groups())==-1)
    {
      write("No such newsgroup\n");
      return 0;
    }
  if(SAVE_OB->check_subscribed(group))
    {
      SAVE_OB->unsubscribe_group(group);
      printf("Group %s unsubscribed.\n",group);
      return;
    }
  SAVE_OB->subscribe_group(group);
  printf("Group %s subscribed\n",group);
}

private nomask void global_commands(string cmd)
{
  cmd=trim_spaces(cmd);
  if( cmd == "q" )
    {
        quit_news();
    }
    else if ( cmd == "s" )
    {
        modal_simple((: receive_search, 0 :), "Search for message: ");
    }
    else if ( cmd == "S" )
    {
        modal_simple((: receive_search, 1 :), "Search for author: ");
    }
    else if ( cmd == "")
    {
        if ( sizeof(filter_array(SAVE_OB->subscribed_groups(),
              (: test_for_new :))) )
        {
            modal_pop();
            begin_reading();
            return;
        }
        else
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
        SAVE_OB->catch_up_newsgroup(current_group);
        write("All posts marked as read.\n");
        next_group();
    }
    else if ( cmd == "h")
    {
        modal_simple((: menu_select_change_header :), "Change subject to: ");
    }
    else if (cmd=="u")
      {
	toggle_subscription(current_group);
      }
    else if ( cmd == "" )
    {
      next_message();
      if(current_id)
	read_message(current_group,current_id);  
    }
    else if( id = to_int(trim_spaces(cmd)) )
    {
        if ( id < 1 )
            id = 1;
        else if ( id > NEWS_D->get_group_last_id(current_group) )
	  {
	    current_id=id;
	  }
	current_id=id;
	read_message(current_group,current_id);
    }
    else
    {
        global_commands(cmd);
    }
}

private nomask void receive_remove_verify(string str)
{
    if ( str[0] != 'y' && str[0] != 'Y' )
    {
        write("Removal aborted.\n");
        return;
    }
    NEWS_D->remove_post(current_group, current_id);
}

private nomask void get_move_group(string str)
{
    string * matches;
    int i;

    matches = complete (str, NEWS_D->get_groups());
    switch ( sizeof(matches) )
    {
    case 0:
        write("No such group.\n");
        return;
    case 1:
        NEWS_D->move_post(current_group,
			  current_id,
			  matches[0]);
        break;
    default:
        write("\n"
          "Select group by number:\n"
          "--------------------------------\n");
        for ( i = 1; i <= sizeof(matches); i++)
            printf("%-4d%s\n",i,matches[i-1]);
        group_selection_menu_items = matches;
        modal_func((: menu_select_movegroup:), "[#q] ");
        return;
    }
}

private nomask void receive_move_verify( string str )
{
    if( str[0] != 'y' && str[0] != 'Y' )
    {
        write( "Move aborted.\n");
        return;
    }
    modal_simple( (: get_move_group :), "Move post to: ");
}

private nomask void remove_message()
{
    class news_msg msg = ((class news_msg)NEWS_D->get_message(current_group,current_id));

    if ( !msg )
    {
        write("This post has already been removed.\n");
        return;
    }
    if ( !adminp(this_user()) &&
      msg->userid != this_user()->query_userid() )
    {
        write("You are not allowed to remove that post.\n");
        return;
    }

    printf("Deleting: %s\nAre you sure? [yn] > ",
      format_message_line(1, current_id));
    modal_simple((: receive_remove_verify :));
}


private nomask void receive_top_cmd(mixed cmd)
{
    if ( cmd == -1 )
        destruct(this_object());

    cmd = trim_spaces(cmd);
    if ( cmd == "?" )
    {
        write("\n" +
          "This is the prompt for commands that apply before "
          "you have picked a newsgroup.  The commands that "
          "apply are:" +
          "\n"
          "  q   quit reading news\n"
          "  ?   this help\n"
          "  l   list newsgroups\n"
          "  s   search for post\n"
          "  S   search for author\n"
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
        more(list);
    }
    else
    {
        global_commands(cmd);
    }
}

private nomask void receive_grp_cmd(mixed cmd)
{
    if ( cmd == -1 )
        destruct(this_object());
    cmd = trim_spaces(cmd);
    if ( cmd == "?" )
      {
        write("\n"
	      "This is the prompt for commands that apply to the "
	      "current newsgroup you are reading (" + current_group +
	      ").  The commands that apply are:"
	      "\n"
	      "  q   quit reading news\n"
	      "  ?   this help\n"
	      "  l   list new messages\n"
	      "  L   list all messages\n"
	      "  m   main news menu\n"
	      "  g   go to a newsgroup\n"
	      "  s   search for a message\n"
	      "  S   search for an author\n"
	      "  p   post message\n"
	      "  u   Toggle subscription to this group\n"
	      "  n   next newsgroup with new news\n"
	      "  c   mark all messages as read (catch up)\n"
	      "\n"
	      "  g group   goes to newsgroup \"group\"\n"
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

private nomask void receive_msg_cmd(mixed cmd)
{
    if (cmd == -1) {
        destruct(this_object());
        return;
    }
    cmd = trim_spaces(cmd);
    switch(cmd)
      {
      case "?":
        write("\n"
              "This is the prompt for commands that apply to the "
              "current newsgroup you are reading (" + current_group +
              ").  The commands that apply are:"
              "\n"
              "  q   quit reading news\n"
              "  ?   this help\n"
              "  l   list new messages\n"
              "  L   list all messages\n"
              "  m   main news menu\n"
              "  g   go to a newsgroup\n"
              "  h   change subject of a post\n"
              "  s   search for a message\n"
              "  S   search for an author\n"
              "  p   post message\n"
              "  r   reply to this message\n"
              "  R   reply to this message, quoting it\n"
              "  f   post a followup to this message\n"
              "  F   post a followup to this message, quoting it\n"
              "  n   next newsgroup with new news\n"
              "  c   mark all messages as read (catch up)\n"
              "  M   Move post to a different group [owner/admin]\n"
              "  D   remove this message\n"
	      "  u   Toggle subscription to this group\n"
              "  g group   goes to newsgroup \"group\"\n"
              "  <return>  read messages or go to next newsgroup with new news\n"
              "\n"
              "Just type a command at the prompt and hit return.\n");
        break;
      case "D":
        remove_message();
        break;
      case "r":
        reply_to_message();
        break;
      case "R":
        reply_with_message();
        break;
      case "f":
        followup_to_message();
        break;
      case "F":
        followup_with_message();
        break;
      case "s":
        modal_simple((: receive_search :), "Search for message: ");
        break;
      case "M":
        modal_simple((: receive_move_verify :), "Move post? [y/N] ");
        break;
      default:
        group_commands(cmd);
      }
}

nomask void convert_news()
{
  if(!SAVE_OB->get_news_data())
    {
      SAVE_OB->set_news_data(OLD_SAVE_OB->get_news_data());
      write("Converting news.....\n");
    }
}


varargs nomask void begin_reading(string arg)
{
  convert_news();
    if ( !sizeof(NEWS_D->get_groups()) )
    {
      printf( "%s has no newsgroups right now.\n", mud_name());
      destruct();
      return;
    }
    SAVE_OB->validate_groups();
    if ( sizeof(arg) && arg[0] != '-' )
    {
      modal_push((: receive_grp_cmd :), (: grp_cmd_prompt :));
      read_group(arg);
      return;
    }
    add_new_groups();
}
