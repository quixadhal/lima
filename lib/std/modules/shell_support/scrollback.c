#include <mudlib.h>

private static string scrollback = "";
private static string last_scrollback = 0;

private nomask void cmd_scrollback()
{
  if(!last_scrollback)
    {
      write("You have no scrollback.\n");
      return;
    }
  new(MORE_OB)->more_string(last_scrollback);
}


static void reconnect()
{
  last_scrollback = scrollback;
  scrollback = "";
}

nomask void add_scrollback(string s)
{
// The shell should really have an owner object.
  if(base_name(previous_object()) != DEV && 
     base_name(previous_object()) != PLAYER)
    return;

  scrollback += s;
}
