/* Do not remove the headers from this file! see /USAGE for more info. */

inherit __DIR__ "death_messages";

void remove();
void simple_action(string);
string query_name();
varargs void stop_fight(object);
void stop_attacking();

private string corpse_long;
private string corpse_filename = CORPSE;

nomask string query_default_death_message()
{
   return "$N $vbreathe $p last breath and $vslump to the ground, dead.";
}

//:FUNCTION set_corpse_long
// void set_corpse_long(string str);
// Sets the long description of this adversary's corpse.
void set_corpse_long(string str)
{
   corpse_long = str;
}

//:FUNCTION set_corpse_filename
// void set_corpse_filename(string str);
// Sets the corpse to be cloned to 'str'.
void set_corpse_filename(string str)
{
   corpse_filename = str;
}

//:FUNCTION drop_corpse
// Replace us with a corpse.
void drop_corpse()
{
   object corpse = new(corpse_filename, query_name(), corpse_long);
   all_inventory()->set_worn(0);
   all_inventory()->move(corpse);
   corpse->move(environment());
}

protected void die()
{
   stop_fight(0);
   stop_attacking();
   simple_action(query_death_message());
   drop_corpse(); // subtle pun
   call_out("remove", 1);
}
