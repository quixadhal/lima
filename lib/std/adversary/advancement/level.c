/* Do not remove the headers from this file! see /USAGE for more info. */

void save_me();

private int level = 1;

void set_level(int x)
{
   level = x;
   save_me();
}

int query_level()
{
   return level;
}
