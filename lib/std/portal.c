/* Do not remove the headers from this file! see /USAGE for more info. */

//:MODULE portal
// A simple one way portal

inherit OBJ;

string look_in_desc;
string portal_destination;

void do_enter()
{
  this_body()->move_to (portal_destination, "through " + the_short ());
}

//:FUNCTION set_destination
// Set where you go when you enter the portal
void set_destination (string s)
{
  portal_destination = s;
}

string get_destination ()
{
  return portal_destination;
}

int direct_look_str_obj()
{
  return 1;
}

//:FUNCTION set_look_in_desc
// Set what you see when you look in the portal.
void set_look_in_desc (string s)
{
  look_in_desc = s;
}

string get_look_in_desc ()
{
  return look_in_desc;
}

string look_in(string prep)
{
  return look_in_desc;
}

int direct_enter_obj()
{
  return 1;
}
