/* Do not remove the headers from this file! see /USAGE for more info. */

//
// FOLLOWUP.C    970408  Fritti
//
// Followup a post on a bulletin board.
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

private void receive_body(object brd, int followup_id, string array body) {
    if (!body) {
        write("Followup aborted.\n");
        return;
    }

    NEWS_D->followup(brd->query_group(), followup_id, implode(body, "\n"));
    write("Posted.\n");
}

private void main(string arg) {
    object ob, brd;
    int array ids;
    int id;

    brd = 0;
    foreach (ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    if (!brd) {
        write("There is nothing here to post on.\n");
        return;
    }

    if (!NEWS_D->query_write_to_group(brd->query_group())) {
        write("You can't write on the bulletin board.\n");
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

    new(EDIT_OB, EDIT_TEXT, 0, (: receive_body, brd, ids[id - 1] :));
}
