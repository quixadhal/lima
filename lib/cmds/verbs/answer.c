/* Do not remove the headers from this file! see /USAGE for more info. */

//
// ANSWER.C    970414  Fritti
//
// Reply to a user about a post on a bulletin board.
//

//:COMMAND
// Mainly used to mail a poster an answer to a posting
// See: boards

inherit NVERB_OB;

mixed can_answer_to_wrd(string wrd) {
    object brd = 0;
    mixed res;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board()) {
            if (!brd)
                brd = ob;
            else
                return "Please specify which board you want to reply on.\n";
        }
    }
    if (!brd)
        return "There is nothing here to reply to.\n";

    if ((res = brd->direct_answer_to_wrd_on_obj(wrd, brd)) != 1)
        return res;

    return default_checks();
}

mixed can_answer_to_wrd_on_obj(string wrd, object ob) {
    return default_checks();
}

void do_answer_to_wrd(string wrd) {
    object brd = 0;
    int id;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    sscanf(wrd, "%d", id);
    brd->do_reply(id);
}

void do_answer_to_wrd_on_obj(string wrd, object ob) {
    int id;
    
    sscanf(wrd, "%d", id);
    ob->do_reply(id);
}

void create() {
    add_rules( ({ "to WRD", "to WRD on OBJ" }), ({ }) );
}
