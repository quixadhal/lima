/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * By Krithtik@Lima Bean
 */
inherit M_THROWABLE;
inherit FOOD;

mixed throw (object target) {
    this_body()->targetted_action("$N $vthrow a golden twinkie at $t.",target);
    target->simple_action("$N $vare covered in a creamy filling!");
    destruct(this_object());
    return 1;
}

void setup() {
    set_adj("golden");
    set_id("twinkie");
    set_long("The pale yellow twinkie hums softly before you."); 
    set_gettable(1);
    set_num_eats(3);
    set_eat_action("$N $veat the golden sponge cake.");
    set_last_eat_action( "$N $vpop the last of the golden sponge cake into $p mouth, and $vchew contentedly.");

}
