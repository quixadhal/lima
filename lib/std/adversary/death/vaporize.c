/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "death_messages";

void remove();
void simple_action(string);

nomask string query_default_death_message()
{
   return "Soon after $n $vhave breathed $p last, $p body disappears in a puff of black smoke.";
}

//:FUNCTION vaporize
// void vaporize();
// Remove the monster in a puff of black smoke!
void vaporize()
{
   simple_action(query_death_message());
   filter(all_inventory(this_object()), (: $1->is_armor() :))->remove_it();
   all_inventory(this_object())->move(environment());
   remove();
}

protected void die()
{
   call_out((: vaporize :), 1);
}

void drop_corpse()
{
   /* Cheesy hack; body.c calls drop_corpse() */
}
