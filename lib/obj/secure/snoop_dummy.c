/* Do not remove the headers from this file! see /USAGE for more info. */

object array snoopers = ({});
string buf = "";
string header;

void remove()
{
  if(sizeof(snoopers)) 
    {
      error("can't remove while in use.\n");
    }
  destruct();
}

void create(string name)
{
  header = name + "% ";
}

object array get_snoopers()
{
  if(previous_object() != find_object(SNOOP_D))
    {
      return;
    }
  return snoopers;
}

void add_snooper(object snooper)
{
  if(previous_object() != find_object(SNOOP_D))
    {
      return;
    }
  snoopers += ({ snooper });
}

void remove_snooper(object snooper)
{
  if(previous_object() != find_object(SNOOP_D))
    {
      return;
    }
  snoopers -= ({ snooper });
  if(!sizeof(snoopers))
    {
      remove();
    }
}

void receive_snoop(string s)
{
  if(!strlen(s)) return;
  // the m is part of a control sequence for some people
  if(s[<1] == '\n' || (s[<1] == 'm' && strsrch(s,"\n")!=-1))
    {
      snoopers->receive_private_msg(header + buf + s);
      buf = "";
    }
  else
    {
      buf += s;
    }
}

