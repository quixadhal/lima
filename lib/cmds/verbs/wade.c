/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** get.c
**
*/

#include <mudlib.h>
#include <move.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_wade_in_obj(object ob) {
    return 1;
}

mixed can_wade()
{
  return 1;
}

void do_wade()
{
  mixed s = environment(this_body())->wade();

  if(stringp(s))
    {
      write(s);
      return;
    }
  else if(!s)
    {
      write("I don't understand what you want to wade in.\n");
      return;
    }
}



void do_wade_in_obj(object ob, string str) {
  mixed s;
    s = ob->wade(str);
    if(stringp(s))
      {
	write(s);
	return;
      }
    if(!s)
      {
	write("You can't wade in that!\n");
      }
}

int direct_wade_in_obj(object ob)
{
  return 1;
}


mixed * query_verb_info()
{
    return ({ ({ "", "in OBJ", }),({  }) });

}


