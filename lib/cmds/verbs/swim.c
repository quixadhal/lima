/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** get.c
**
*/

#include <mudlib.h>
#include <move.h>
#include <setbit.h>

inherit VERB_OB;

void do_swim()
{
  mixed s = environment(this_body())->swim();

  if(stringp(s))
    {
      write(s);
      return;
    }
  else if(!s)
    {
      write("I don't understand what you want to swim in.\n");
      return;
    }
}



void do_swim_in_obj(object ob, string str) {
  mixed s;
    s = ob->swim(str);
    if(stringp(s))
      {
	write(s);
	return;
      }
    if(!s)
      {
	write("You can't swim in that!\n");
      }
}

int direct_swim_in_obj(object ob)
{
  return 1;
}


mixed * query_verb_info()
{
    return ({ ({ "", "in OBJ", }),({  }) });

}


