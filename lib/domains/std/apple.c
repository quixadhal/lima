/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_EDIBLE;

void
setup()
{
   set_id("apple");
   set_long("An ordinay apple.\n");
   set_eat_action("$N $vtake a bite from a apple.\n");
set_last_eat_action( (:destruct(this_object()) :) );
set_num_eats(1);
}
