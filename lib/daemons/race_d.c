/* Do not remove the headers from this file! see /USAGE for more info. */

mapping races = ([ ]);
int width = 0;

void create()
{
   foreach(string file in get_dir(DIR_RACES + "/*.c"))
   {
      string tmp = DIR_RACES + "/" + file;
      string name;

      if(!load_object(tmp))
         continue;
      name = tmp->query_race();
      races[name] = tmp;
      if(strlen(name) > width)
         width = strlen(name);
   }
}

string query_race_filename(string race)
{
   return races[race];
}

string array query_races()
{
   return keys(races);
}

mapping query_race_data()
{
   return copy(races);
}
