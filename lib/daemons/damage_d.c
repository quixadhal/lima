/* Do not remove the headers from this file! see /USAGE for more info. */

// Damage daemon
// To store list of valid damage types,
// so weapons and armours check against the list
// (from M_DAMAGE_SOURCE and M_DAMAGE_SINK)
//

inherit M_DAEMON_DATA;

private string array damage = ({ });

void add_damage_type( string t )
{
  if (member_array(t,damage) == -1)
  {
    damage += ({ t });
    save_me();
    write("Damage type " + t + " added\n");
  }
}

void remove_damage_type( string t )
{
  if (member_array(t,damage) == -1)
  {
    damage -= ({ t });
    save_me();
    write("Damage type " + t + " removed\n");
  }
}

void clear_damage_types()
{
  damage = ({ });
  save_me();
  write("Damage types cleared\n");
}

string array query_damage_types()
{
  return copy(damage);
}

int query_valid_damage_type( string str)
{
  if(member_array(str, damage) == -1)
    return 0;
  return 1;
}
