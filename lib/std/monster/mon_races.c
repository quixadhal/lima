/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE
// Monster Race
// Monster races are supported via USE_MON_RACES defined
// Vette  April 15, 1997
//

private nosave string monster_race;

string query_mon_race() {
  return monster_race;
}

void set_mon_race(string str) {
  monster_race = str;
}

string query_race() {
  return query_mon_race();
}
