/* Do not remove the headers from this file! see /USAGE for more info. */

//
// BOARD.C    970408 Fritti
//
// Generic bulletin board.
//
// Public interface (aka how to use this thing):
//   object ob = new(BOARD, "Your board's name", "linked.newsgroup");
//   ob->move(where_ever_you_want);
//

inherit OBJ;
inherit M_READABLE;
inherit CLASS_NEWSMSG;

// The group to which this board is linked. Set by setup().
private nosave string linked_group;

// The name of this board. Set by setup().
private nosave string board_name;

// Function prototypes.
void set_group(string new_group);
private nomask string list_headers();
string do_desc();
int get_group_last_id(string group);

// Filter removed messages from ids array.
private nomask int filter_removed(int elem) {
    class news_msg msg = NEWS_D->get_message(linked_group, elem);
    
    if (!msg || !msg->body)
        return 0;

    return 1;
}

// Create a new board.
nomask void create(string name, string group) {
    ::create();

    if (!clonep(this_object()))
        return;

    if (!group || group == "")
	error("No linked newsgroup in board.\n");

    set_group(group);

    board_name = name;
    set_id("board", "bulletin board");
    set_in_room_desc( (: do_desc :) );
    set_long( (: list_headers :) );
}

// Set and query functions.
private nomask void set_group(string new_group) {
    if (member_array(new_group, NEWS_D->get_groups()) == -1)
	error("Invalid groupname <"+new_group+"> in set_group()\n");

    linked_group = new_group;
}

nomask string query_group() {
    return linked_group;
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
      "%-35s [%-10s on %s]",           // Display message subject/poster/time
      subject[0..34],
      msg->poster,
      intp(msg->time) ? ctime(msg->time)[4..9] : msg->time);
}

// Format all message lines.
private nomask string array query_message_lines() {
    int array ids = sort_array(filter_array(
      NEWS_D->get_messages(linked_group), (: filter_removed :)), 1);
    int i;
    string array tmp;

    tmp = map_array(ids, (: format_message_line($1) :));
    for (i = 1; i < sizeof(tmp) + 1; i++)
        tmp[i-1] = sprintf("%4d. ", i) + tmp[i-1];
    return tmp;
}

// Make the long description.
private nomask string list_headers() {
    return "ID    TITLE                               POSTER      TIME\n" +
      repeat_string("-", 78) + "\n" + implode(query_message_lines(), "\n");
}

// Needed for M_READABLE
nomask int has_entries() {
    return 1;
}

// Read about an entry.
nomask mixed read_entry(string str) {
    class news_msg msg;
    int id;
    int array ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);

    if (str == "next") {
        id = this_body()->query_board_id(board_name) + 1;
        if (id > sizeof(ids))
            return "No more messages.\n";
    }
    else
        if (sscanf(str, "%d", id) != 1)
	    return "Specify the post number you want to read.";

    if (id <= 0 || id > sizeof(ids))
	return "You can only read posts that are on the bulletin board!\n";

    this_body()->set_board_id(board_name, id);
    id = ids[id - 1];                  // Translate to NEWS_D id
    msg = NEWS_D->get_message(linked_group, id);

    return format_message_line(id) + "\n\n" + msg->body;
}

// Helpful text for those used to type 'read 1'.
nomask mixed direct_read_obj(object ob) {
    return "Use 'read about <postnr>' to read the bulletin board.\n";
}

// Short description.
private nomask string do_desc() {
    int array ids = sort_array(filter_array(NEWS_D->get_messages(linked_group),
      (: filter_removed :)), 1);
    int curr_id = this_body()->query_board_id(board_name);

    if (curr_id <= 0)
        this_body()->set_board_id(board_name, curr_id = 1);
    if (curr_id > sizeof(ids))
        this_body()->set_board_id(curr_id = sizeof(ids));

    return "A " + board_name + " [at " + curr_id + ", max " +
      sizeof(ids) + "] is hanging on the wall.";
}

// For board commands like post etc.
nomask int is_bulletin_board() {
    return 1;
}
