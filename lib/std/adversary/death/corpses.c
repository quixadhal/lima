/* Do not remove the headers from this file! see /USAGE for more info. */

void remove();
void simple_action(string);
string query_name();
varargs void stop_fight(object);
void stop_attacking();

private string corpse_long;
private string corpse_filename = CORPSE;

void set_corpse_long(string str)
{
   corpse_long = str;
}

void set_corpse_filename(string str)
{
   corpse_filename = str;
}

//:FUNCTION drop_corpse
//replace us with a corpse
void drop_corpse()
{
   object corpse = new(corpse_filename, query_name(), corpse_long);
   all_inventory()->move(corpse);
   corpse->move(environment());
}

void die()
{
   stop_fight(0);
   stop_attacking();
   simple_action("$N $vbreath $p last breath and $vslump to the ground, dead.");
   drop_corpse(); // subtle pun
   call_out("remove", 1);
}
