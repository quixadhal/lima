/* Do not remove the headers from this file! see /USAGE for more info. */

//
// DELETE.C    970408  Fritti
//
// Delete a post from a bulletin board.
//

inherit CMD;
inherit CLASS_NEWSMSG;

private int filter_removed(object brd, int elem) {
    class news_msg msg = NEWS_D->get_message(brd->query_group(), elem);

    if (!msg || !msg->body)
        return 0;

    return 1;
}

private void main(string arg) {
    object ob, brd;
    int array ids;
    int id;
    class news_msg msg;

    brd = 0;
    foreach (ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    if (!brd) {
        write("There is nothing here to delete notes from.\n");
        return;
    }

    if (!arg || arg == "" || sscanf(arg, "%d", id) != 1) {
        write("Usage: delete <postnr>.\n");
        return;
    }

    if (!NEWS_D->query_write_to_group(brd->query_group())) {
        write("You don't have permission to delete notes from this board.\n");
        return;
    }

    ids  = sort_array(filter_array(NEWS_D->get_messages(brd->query_group()),
      (: filter_removed, brd :)), 1);

    if (id <= 0 || id > sizeof(ids)) {
        write("You can only delete posts that are on the bulletin board!\n");
        return;
    }
    msg = NEWS_D->get_message(brd->query_group(), ids[id - 1]);

    if (msg->poster != this_body()->query_name()) {
        write("You can only delete your own posts.\n");
        return;
    }

    NEWS_D->remove_post(brd->query_group(), ids[id - 1]);
    write("Post nr. " + id + " removed.\n");
}
