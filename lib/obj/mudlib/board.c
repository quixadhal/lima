/* Do not remove the headers from this file! see /USAGE for more info. */

//
// BOARD.C    970414 Fritti@TiamaT, Vette@Lima
//
// Generic bulletin board, with switching group capability.
//
// Added security checks using NEWS_D internal security
// Vette April 14, 1997

//:MODULE
//  Boards are simply set up from rooms by using
//
//   set_objects( ([
//      BOARD_OB : ({ "name", "linked newsgroup", int switchable })
//    ]) );

// TODO
// Something to show more than the MAX_HEADERS amount of posts
// The query_message_lines() returns an array which is then imploded,
//   would it not be faster to send a string and not have to implode?
 
#include <edit.h>

inherit OBJ;
inherit M_READABLE;
inherit M_INPUT;
inherit CLASS_NEWSMSG;

// The maximum nr of headers to show.
#define MAX_HEADERS             10 

// Default to no switchable boards.
#define DEFAULT_SWITCHABLE      0

// The default mud newsgroup
#define DEFAULT_GROUP NEWS_D->get_groups()[1]

// The default Name of the Board Object
#define DEFAULT_NAME "Bulletin Board"

// The group to which this board is linked. Set by setup().
private static string linked_group;

// The name of this board. Set by setup().
private static string board_name;

// Is this board switchable? Set by create().
private int can_switch;

// Function prototypes.
varargs private nomask int get_current_id(string group_name);
int set_group(string new_group);
private nomask string list_headers();
string do_desc();
int get_group_last_id(string group);
private nomask string switch_to_next_unread_group();

// Filter removed messages from ids array.
private nomask int filter_removed(int elem) {
    class news_msg msg = NEWS_D->get_message(linked_group, elem);
    
    if (!msg || !msg->body)
        return 0;

    return 1;
}

// Create a new board.
varargs void mudlib_setup( string name, string group, int switchable )
{
    if (!group || group == "")
        group = DEFAULT_GROUP;
    if (!name || name == "")
        name = DEFAULT_NAME;
    if (!switchable)
        switchable = DEFAULT_SWITCHABLE;

    can_switch = switchable;

    set_group(group);

    board_name = name;
    set_id("board", "bulletin board");
    set_in_room_desc( (: do_desc :) );
    set_long( (: list_headers :) );
    set_size( SMALL );
}

// A check to test whether the current user is allowed to read
// from this group.
// Added by Vette  April 14, 1997
private nomask int check_group(string g) {
    if (member_array(g, NEWS_D->get_groups()) == -1) {
      return 0;
    }
    return 1;
}

// Set and query functions.
// Moved the group check into its own function and called that fn
// Vette  April 14, 1997
private nomask int set_group(string new_group) {
    if (!check_group(new_group)) return 0;
    linked_group = new_group;
    return 1;
}

// Added Valid group check
nomask string query_group() {
    if (!check_group(linked_group)) return DEFAULT_GROUP;
    return linked_group;
}

// Used for switchable boards.
nomask void set_switchable(int flag) {
    can_switch = flag;
}

nomask int query_switchable() {
    return can_switch;
}

// Format one message line
private nomask varargs string format_message_line(int id) {
    class news_msg msg;
    string subject;

    msg = NEWS_D->get_message(linked_group, id);

    if (!msg || !msg->body)
        return 0;                      // Do not display removed messages.
    else
	subject = msg->subject;

    return sprintf(
      "%-48s [%-10s on %s]",           // Display message subject/poster/time
      subject[0..47],
      msg->poster,
      intp(msg->time) ? ctime(msg->time)[4..9] : msg->time);
}

// Format all message lines.
// Added some changes to support groups with less than MAX_HEADERS posts
// to them...it used to make negative ID's
// Added a * to unread posts
private nomask string array query_message_lines() {
    int array ids = sort_array(filter_array(
      NEWS_D->get_messages(linked_group), (: filter_removed :)), 1);
    int i,k, j = sizeof(ids) - MAX_HEADERS;
    string array tmp;

    if (j<0) j = 0;
//### This can probably be removed, but will wait for now  --Vette--
//###    ids = ids[<MAX_HEADERS..];
    ids = ids[j..];
    tmp = map_array(ids, (: format_message_line($1) :));
    k = ids[<1]-this_body()->get_news_group_id(linked_group);
    for (i = 1; i < sizeof(tmp) + 1; i++) {
        tmp[i-1] = sprintf("%4d%s ", i + j, k>(sizeof(tmp)-i-1) ? "*" : " ") + tmp[i-1];
    }
    return tmp;
}

// Make the long description.
private nomask string list_headers() {
    if (!check_group(linked_group)) 
      return "You do not have permission.\n";

    return (can_switch ? "Group: " + linked_group + "\n" : "") +
           "ID    TITLE                                            POSTER      TIME\n" +
      repeat_string("-", 78) + "\n" + implode(query_message_lines(), "\n");
}

// Needed for M_READABLE
nomask int has_entries() {
    return 1;
}

// Read about an entry.
// Added a couple of changes to beef up security
// Vette  April 14, 1997
nomask mixed read_entry(string str) {
    class news_msg msg;
    int id;
    int array ids;

// Before we do anything, need to make sure this user can read the group
    if (!check_group(linked_group)) 
      return "You do not have access.\n";

    ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);

    if (str == "next") {
        id = this_body()->get_news_group_id(linked_group) + 0;
        while(member_array(id,ids) == -1  &&  id < ids[sizeof(ids)-1])
          id++;
        /* This ID already a NEWS_D ID, no translation needed */
        if (id > ids[sizeof(ids)-1])
            return "No more messages.\n";
        if (id < ids[0] || id > ids[sizeof(ids)-1])
            return "You can only read posts that are on the bulletin board!\n";
    }
    else {
        if (sscanf(str, "%d", id) != 1)
	    return "Specify the post number you want to read.";
        if (id < 0 || id > sizeof(ids))
            return "You can only read posts that are on the bulletin board!\n";
        id = ids[id - 1];                  // Translate to NEWS_D id
    }

    if (this_body()->get_news_group_id(linked_group) < (id+1))
      this_body()->set_news_group_id(linked_group, id+1);
    msg = NEWS_D->get_message(linked_group, id);

    return format_message_line(id) + "\n\n" + msg->body;
}

// Helpful text for those used to type 'read 1'.
nomask mixed direct_read_obj(object ob) {
    return "Use 'read about <postnr>' to read the bulletin board.\n";
}

// Short description.
private nomask string do_desc() {
    int new_id;
    int array ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);
    int curr_id = this_body()->get_news_group_id(linked_group);

/* Slight fix here, lest see if this solves our problems. */
    if (curr_id < 0) curr_id=1;
/*
    if (curr_id <= 0)
        this_body()->set_news_group_id(linked_group, curr_id = 1);
    if (curr_id > sizeof(ids))
        this_body()->set_news_group_id(linked_group, curr_id = sizeof(ids));
*/

    new_id = member_array(curr_id,ids);
    if (curr_id > 0 && new_id < 0) new_id = sizeof(ids);
    else if (new_id < 0) new_id = 1;
    return "A " + board_name + (can_switch ? " set to " + linked_group : "") +
      " [at " + new_id + ", max " + sizeof(ids) + "].";
}

// For board verbs like post etc.
nomask int is_bulletin_board() {
    return 1;
}

// Check if id is in range, for board verbs can_* functions.
nomask int valid_id(int id) {
    int array ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);
    if (id <= 0 || id > sizeof(ids))
        return 0;

    return 1;
}

// Check if this_body() can post.
nomask int valid_post() {
    return NEWS_D->query_write_to_group(linked_group);
}

// Verb interface. These get called by their respective verbs.

// Remove a note.
nomask void do_remove(int id) {
    int array ids;
    class news_msg msg;

    ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);
    msg = NEWS_D->get_message(linked_group, ids[id - 1]);

    if (msg->poster != this_body()->query_name() && !check_privilege(1)) {
        write("You can only delete your own posts.\n");
        return;
    }

    NEWS_D->remove_post(linked_group, ids[id - 1]);
    write("Post nr. " + id + " removed.\n");
    return;
}

mixed direct_remove_wrd_from_obj(string wrd, object ob) {
    int id;

    if (!valid_post())
        return "You aren't permitted to remove posts from this board.\n";

    if (sscanf(wrd, "%d", id) != 1)
        return "Please specify a post number.\n";
        
    if (!valid_id(id))
        return "No such note.\n";

    return 1;
}

// Post a note.
private void receive_post(string subj, string array body) {
    if (!body) {
        write("Post aborted.\n");
        return;
    }

    NEWS_D->post(linked_group, subj, implode(body, "\n"));
    write("Posted.\n");
}

private void receive_subject(mixed subj) {
    if (subj == -1)
        return;

    if (!subj || subj == "") {
        write("No subject, post aborted.\n");
        modal_pop();
        return;
    }

    if (sizeof(subj) > 50) {
        write("Subject too long. Please try again.\n");
        return;
    }

    modal_pop();
    new(EDIT_OB, EDIT_TEXT, 0, (: receive_post, subj :));
}

nomask void do_post(string subj) {
    if (subj == "" || !subj)
        modal_push( (: receive_subject :), "Subject: ");
    else
        new(EDIT_OB, EDIT_TEXT, 0, (: receive_post, subj :));
}

mixed direct_post_on_obj_about_str(string str, object ob) {
    if (!valid_post())
        return "You aren't permitted to post on this board.\n";
    if (!check_group(linked_group))
        return "You lack the permissions.\n";

    if (sizeof(str) > 50)
        return "Subject too long.\n";
        
    return 1;
}


// Followup a note.
private void receive_followup(int followup_id, string array body) {
    if (!body) {
        write("Followup aborted.\n");
        return;
    }

    NEWS_D->followup(linked_group, followup_id, implode(body, "\n"));
    write("Posted.\n");
}

nomask void do_followup(int id) {
    int array ids;

    ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);

    new(EDIT_OB, EDIT_TEXT, 0, (: receive_followup, ids[id - 1] :));
}

nomask void do_followup_with_message(int id) {
    int array ids;
    class news_msg msg;
    string * lines;

    ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);

    msg = NEWS_D->get_message(linked_group,ids[id-1]);
    if ( !msg ) {
        write("You may not followup to that message -- it was removed.\n");
        return;
    }

    lines = ({ sprintf("On %s %s wrote post #%d:",
        intp(msg->time) ? ctime(msg->time) : msg->time,
        msg->poster, get_current_id()) });

    lines += map_array(explode(msg->body, "\n"), (: "> " + $1 :) );

    new(EDIT_OB, EDIT_TEXT, lines, (: receive_followup, ids[id - 1] :));
}

mixed direct_post_wrd_to_wrd_on_obj(string wrd1, string wrd2, object ob) {
    int id;

    if (!valid_post())
        return "You're not allowed to post on this board.\n";
    if (!check_group(linked_group)) 
        return "You lack the permissions.\n";
    if (sscanf(wrd2, "%d", id) != 1)
        return "Please specify a post number.\n";

    if (!valid_id(id))
        return "No such note.\n";

    return 1;
}

mixed direct_post_wrd_with_wrd_on_obj(string wrd1, string wrd2, object ob) {
    return direct_post_wrd_to_wrd_on_obj(wrd1,wrd2,ob);
}

// Reply to a user about a note.
private void receive_reply(int reply_id, string array body) {
    class news_msg msg = NEWS_D->get_message(linked_group, reply_id);

    if (!body) {
        write("Reply aborted.\n");
        return;
    }

    // This line required a change in the mailer
    //  (/obj/secure/mailers/mailer.c) because it checked whether the
    //  previous object was the newsreader.
    this_body()->query_mailer()->send_news_reply("Re: " + msg->subject,
      body, lower_case(msg->poster));

    write("Replied to "+lower_case(msg->poster)+".\n");
}

nomask void do_reply(int id) {
    int array ids;

    ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);

    new(EDIT_OB, EDIT_TEXT, 0, (: receive_reply, ids[id - 1] :));
}

mixed direct_answer_to_wrd_on_obj(string wrd, object ob) {
    int id;

    if (!check_group(linked_group)) 
        return "You lack the permissions.\n";
    if (sscanf(wrd, "%d", id) != 1)
        return "Please specify a post number to reply to.\n";

    if (!valid_id(id))
        return "No such note.\n";

    return 1;
}

// Switch board newsgroup.
nomask void do_switch(string group) {
    if (group == "next")
      if ((group = switch_to_next_unread_group()) == "" || !group) {
        write("No new news.\n");
        return;
      }
    if (!set_group(group)) {
        write("Couldn't change to group <" + group + ">.\n");
        return;
    }
    
    this_body()->simple_action("$N $vchange " + the_short() + " to group <" + group + ">.\n");
}

mixed direct_switch_obj_to_str(object ob, string str) {
    if (!can_switch)
        return 0;

    if (!valid_post())
        return "You aren't permitted to switch this board.\n";

    if ((member_array(str, NEWS_D->get_groups(str)) == -1) && (str != "next"))
        return "That is not a valid newsgroup name.\n";

    return 1;
}

/*
=============================================================================
*/
/* Functions to Support Switching to Next Unread and Listing Unread Groups */

/* Lifted from newsreader.c */
varargs private nomask int get_current_id(string group_name)
{
    if ( !group_name )
        group_name = linked_group;

    /*
    ** -1 to get the "current" message.  The body records the _next_
    ** message to read.
    */
    return this_body()->get_news_group_id(group_name) - 1;
}

/* Lifted from newsreader.c */
private int count_unread_messages(string group, int all_messages)
{
    int * ids = NEWS_D->get_messages(group);
    int id;
    class news_msg msg;
    int count = 0;
    int read_thru_id;

    read_thru_id = get_current_id( group );

    if ( !all_messages )
    {
        ids = filter_array(ids, (: $1 > $(read_thru_id):));
    }
    foreach ( id in ids )
    {
        msg = NEWS_D->get_message(group, id);
        if ( msg->body )
        {
            count++;
        }
    }
    return count;
}

/* Lifted from newsreader.c */
private nomask int test_for_new(string group)
{
    if( count_unread_messages( group, 0 )) return 1;
    return 0;
}

/* Lifted from newsreader.c */
private nomask string format_group_line(string group)
{
    int last_id;
    int unread = count_unread_messages(group, 1);

    last_id = NEWS_D->get_group_last_id(group);
    return sprintf("  %-40s (%d %s, %d unread)",
                   group,
                   unread,
                   unread == 1 ? "message" : "messages",
                   count_unread_messages(group, 0));
}

/* Lifted from newsreader.c (slightly modified) */
private nomask void display_groups_with_new()
{
    string * groups;

    groups = filter_array(this_body()->subscribed_groups(), (: test_for_new :));

    if(!sizeof(groups))
    {
        write("No new news.\n");
    }
    else
    {
        string * list;
        list = ({ "Groups with new messages:", "" }) +
            map_array(groups, (: format_group_line :));
        more(list);
    }
}


private nomask string switch_to_next_unread_group()
{
    string * groups;

    groups = filter_array(this_body()->subscribed_groups(), (: test_for_new :));
    if(!sizeof(groups)) {
        return "";
    }
    else {
        return groups[0];
    }
}
