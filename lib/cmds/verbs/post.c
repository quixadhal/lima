/* Do not remove the headers from this file! see /USAGE for more info. */

//
// POST.C    970411  Fritti
//
// Post a note on a bulletin board.
//

//:COMMAND
// Used mainly to post a message to a board
// See also: boards

inherit VERB_OB;

mixed can_post_about_str(string str) {
    object brd = 0;
    mixed res;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board()) {
            if (!brd)
                brd = ob;
            else
                return "Please specify on which board you want to post.\n";
        }
    }

    if (res = brd->indirect_post_on_obj_about_str(str, brd))
        return res;

    return default_checks();
}

mixed can_post_on_obj_about_str(object ob, string str) {
    return default_checks();
}

void do_post_about_str(string str) {
    object brd = 0;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    brd->do_post(str);
}

void do_post_on_obj_about_str(object ob, string str) {
    ob->do_post(str);
}

/* ************************************************** */
/* Followups are posted using 'post followup to <id>' */
mixed can_post_wrd_to_wrd(string wrd1, string wrd2) {
    object brd = 0;
    mixed res;

    if (wrd1 != "followup")
        return 0;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            if (!brd)
                brd = ob;
            else
                return "Please specify the board you want to post on.\n";
    }
    if (!brd)
        return "There is nothing here to post on.\n";

    if ((res = brd->direct_post_wrd_to_wrd_on_obj(wrd1, wrd2, brd)) != 1)
        return res;

    return default_checks();
}

mixed can_post_wrd_to_wrd_on_obj(string wrd1, string wrd2, object ob) {
    if (wrd1 != "followup")
        return 0;

    return default_checks();
}

void do_post_wrd_to_wrd(string wrd1, string wrd2) {
    object brd = 0;
    int id;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    sscanf(wrd2, "%d", id);
    brd->do_followup(id);
}

void do_post_wrd_to_wrd_on_obj(string wrd1, string wrd2, object ob) {
    int id;

    sscanf(wrd2, "%d", id);
    ob->do_followup(id);
}

/* ********************************************************************** */
/* Followups with inclded text are posted using 'post followup with <id>' */

mixed can_post_wrd_with_wrd(string wrd1, string wrd2) {
    object brd = 0;
    mixed res;

    if (wrd1 != "followup")
        return 0;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            if (!brd)
                brd = ob;
            else
                return "Please specify the board you want to post on.\n";
    }
    if (!brd)
        return "There is nothing here to post on.\n";

    if ((res = brd->direct_post_wrd_with_wrd_on_obj(wrd1, wrd2, brd)) != 1)
        return res;

    return default_checks();
}

mixed can_post_wrd_with_wrd_on_obj(string wrd1, string wrd2, object ob) {
    if (wrd1 != "followup")
        return 0;

    return default_checks();
}

void do_post_wrd_with_wrd(string wrd1, string wrd2) {
    object brd = 0;
    int id;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    sscanf(wrd2, "%d", id);
    brd->do_followup_with_message(id);
}

void do_post_wrd_with_wrd_on_obj(string wrd1, string wrd2, object ob) {
    int id;

    sscanf(wrd2, "%d", id);
    ob->do_followup_with_message(id);
}

/* ***************************************************************** */

void create() {
    add_rules( ({ "about STR", "on OBJ about STR",
                 "WRD to WRD", "WRD to WRD on OBJ",
                 "WRD with WRD", "WRD with WRD on OBJ",
           }), ({ }));
}
