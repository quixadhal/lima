/* Do not remove the headers from this file! see /USAGE for more info. */

// Davmar/Nevyn

#include <mudlib.h>

inherit CMD;


private void main( mixed *arg) {
    object ob;
    mixed msg_ob;

    ob = arg[0];
    msg_ob = (ob->short() ? ob : this_object());
    this_body()->do_player_message("destruct", msg_ob);
    destruct(ob);
    return;
}

string the_short() {
    return "something";
}

string a_short() {
    return "something";
}

string short() {
    return "something";
}
