/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** drop.c
**
*/

#include <mudlib.h>
#include <move.h>

inherit VERB_OB;

mixed can_throw_obj(object ob)
{
    return 1;
}

mixed can_throw_obj_at_obj(object ob1, object ob2)
{
  return 1;
}

varargs private nomask void throw_one(object ob, object target)
{
  mixed tmp;

    if(environment(ob) != this_body())
      {
	write("You don't have it.\n");
	return;
      }

    tmp = ob->throw(target);

    if (!tmp) tmp = target ? "That doesn't make a good weapon.\n" : 
      "Someone else might pick it up...\n";

    if (stringp(tmp)) {
	write(tmp);
	return tmp;
    }

}


void do_throw_obj(object ob)
{
    throw_one(ob);
}

void do_throw_obj_at_obj(object to_throw, object target)
{
  throw_one(to_throw, target);
}


mixed * query_verb_info()
{
    return ({ ({ "OBJ:v", "OBS:v", "OBJ:v at OBJ" }) });
}

