/* Do not remove the headers from this file! see /USAGE for more info. */

void save_me();

private int level = 1;
private int experience = 0;

void set_level(int x)
{
   level = x;
   if(this_body()->query_link())
      save_me();
}

int query_level()
{
   return level;
}

int query_experience()
{
   return experience;
}

void add_experience(int x)
{
   experience += x;
   if(this_body()->query_link())
      save_me();
}
