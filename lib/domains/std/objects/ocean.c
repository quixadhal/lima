/* Do not remove the headers from this file! see /USAGE for more info. */


inherit OBJ;

void setup()
{
  set_attached(1);
  set_id("ocean","water","surf","waves");
  set_long("There is nothing special about the ocean.");
}


int wade(string s)
{
  return environment(this_object())->wade(s);
}
