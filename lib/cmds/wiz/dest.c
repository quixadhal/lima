/* Do not remove the headers from this file! see /USAGE for more info. */

// Davmar/Nevyn

#include <mudlib.h>




int main( mixed *arg) {
    object ob;
    mixed msg_ob;

    ob = arg[0];
    msg_ob = (ob->short() ? (mixed)ob : (mixed)"something");
    this_body()->do_player_message("destruct", msg_ob);
    ob->remove();
    if(ob)destruct(ob);
    return 1;
}
