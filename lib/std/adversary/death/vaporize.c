/* Do not remove the headers from this file! see /USAGE for more info. */

void remove();
void simple_action(string);

//:FUNCTION vaporize
//remove the monster in a puff of black smoke
void vaporize()
{
   simple_action("Soon after $n $vhave breathed $p last, $p body disappears in a puff of black smoke.");
   all_inventory(this_object())->move(environment());
   remove();
}

void die()
{
   call_out((: vaporize :), 1);
}

void drop_corpse()
{
   /* Cheesy hack; body.c calls drop_corpse() */
}
