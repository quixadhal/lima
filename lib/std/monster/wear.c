/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * Handle the ability of 'monsters' to wear other objects.
 * Created to support do_wear(object armor) in monster setup()
 *   that parallels do_wield(object weapon)
 */

object give_new_obj( object ob, string str, int recurse );


//:FUNCTION do_wear
//wear the specified object, as well as checking if it is valid and printing
//the appropriate message
int do_wear(object ob) {
    ob->do_wear();
    return 1;
}


//:FUNCTION set_wearing
//clone a new wearable object, move it to the body and force
//the body to wear it.
int set_wearing(string str) {
    object ob = give_new_obj( this_object(), str, 0 );
    return do_wear(ob);
}
