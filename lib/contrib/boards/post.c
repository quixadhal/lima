/* Do not remove the headers from this file! see /USAGE for more info. */

//
// POST.C    970804  Fritti
//
// Post a note on a bulletin board.
//

#include <edit.h>

inherit CMD;
inherit M_INPUT;

private void receive_body(object brd, string subj, string array body) {
    if (!body) {
        write("Post aborted.\n");
        return;
    }

    NEWS_D->post(brd->query_group(), subj, implode(body, "\n"));
    write("Posted.\n");
}

private void receive_subject(object brd, mixed subj) {
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
    new(EDIT_OB, EDIT_TEXT, 0, (: receive_body, brd, subj :));
}

private void main(string arg) {
    object ob, brd;

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
        write("You're not allowed to post on this board.\n");
        return;
    }

    if (arg == "" || !arg)
	modal_push( (: receive_subject, brd :), "Subject: ");
    else
        new(EDIT_OB, EDIT_TEXT, 0, (: receive_body, brd, arg :));
}
