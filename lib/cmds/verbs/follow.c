/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_follow_obj(object ob) {
    this_body()->do_game_command("go " + ob->query_follow_direction());
}

array query_verb_info() {
    return ({ ({ "OBJ" }) });
}
