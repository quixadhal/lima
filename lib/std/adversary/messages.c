/* Do not remove the headers from this file! see /USAGE for more info. */

void simple_action(string msg, array obs...);
varargs mixed *action(mixed *, mixed, object, object);
void inform(mixed *, mixed, object);
string query_combat_message(string);

string damage_message(int percent)
{
  switch (percent)
  {
    case 0: return "!none";
    case 1: return "!dam1";
    case 2..3: return "!dam2";
    case 4..5: return "!dam3";
    case 6..9: return "!dam4";
    case 10..14: return "!dam5";
    case 15..24: return "!dam6";
    case 25..49: return "!dam7";
    case 50..79: return "!dam8";
    case 80..100: return "!dam9";
    default: return "!dam10";
  }
}

void handle_message(string mess, object target, object weapon, string limb)
{
  array combat_who, messages;

  if (mess[0] == '!')
  {
    string tmp;

    if (weapon)
      tmp = weapon->query_combat_message(mess[1..]);
    else
      tmp = query_combat_message(mess[1..]);

    if (!tmp)
    {
      simple_action("$N $vare puzzled because $n $vhave no message for '"+mess[1..]+"'.");
LBUG(mess);
LBUG(target);
LBUG(weapon);
      return;
    }

    mess = tmp;
  }

  combat_who = ({ this_object(), target });
  if(!limb)
    limb = target->query_random_limb();
  messages = action(combat_who, mess, weapon, target->query_weapon(), limb);

  inform(combat_who, messages, environment());
}
