/* Do not remove the headers from this file! see /USAGE for more info. */

//
// REPLYNOTE.C    970408  Fritti
//
// Reply to a user about a post on a bulletin board.
//

#include <edit.h>

inherit CMD;
inherit M_INPUT;
inherit CLASS_NEWSMSG;

private int filter_removed(object brd, int elem) {
    class news_msg msg = NEWS_D->get_message(brd->query_group(), elem);

    if (!msg || !msg->body)
        return 0;

    return 1;
}

private void receive_body(object brd, int reply_id, string array body) {
    class news_msg msg = NEWS_D->get_message(brd->query_group(), reply_id);

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

private void main(string arg) {
    object ob, brd;
    int id;
    int array ids;

    brd = 0;
    foreach (ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    if (!brd) {
        write("There is nothing here to reply to.\n");
        return;
    }

    if (sscanf(arg, "%d", id) != 1) {
        write("Please specify a post number.\n");
        return;
    }

    ids = sort_array(filter_array(NEWS_D->get_messages(brd->query_group()),
      (: filter_removed, brd :)), 1);
    if (id <= 0 || id > sizeof(ids)) {
        write("No such note.\n");
        return;
    }
    id = ids[id - 1];

    new(EDIT_OB, EDIT_TEXT, 0, (: receive_body, brd, id :));
}
