/* Do not remove the headers from this file! see /USAGE for more info. */

//
// SWITCH.C    970411  Fritti
//
// Switch newsgroups on a board.
// Inspired by Vette's command.
//

//:COMMAND
// Mainly used to switch which group a board is set to
// See also: boards

inherit VERB_OB;

mixed can_switch_to_str(string str) {
    object brd = 0;
    mixed res;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            if (!brd)
                brd = ob;
            else
                return "Please specify which board you want to switch.\n";
    }
    if (!brd)
        return "There is nothing here to switch groups on.\n";

    if ((res = brd->direct_switch_obj_to_str(brd, str)) != 1)
        return res;

    return default_checks();
}

mixed can_switch_obj_to_str(object ob, string str) {
    return default_checks();
}

void do_switch_to_str(string str) {
    object brd = 0;

    foreach (object ob in deep_inventory(environment(this_body()))) {
        if ((int)ob->is_bulletin_board())
            brd = ob;
    }
    brd->do_switch(str);
}

void do_switch_obj_to_str(object ob, string str) {
    ob->do_switch(str);
}

array query_verb_info() {
    return ({ ({ "to STR", "OBJ to STR" }) });
}
