/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_PARSING;

mixed can_unlock_obj(object ob)
{
return 1;
}
 
mixed can_unlock_obj_with_obj(object ob)
{
    return 1;
}


void do_unlock_obj_with_obj(object ob1, object ob2) {
if (!ob1->unlock_with(ob2))
   write(useless ("YOu can't unlock that"));
} 

mixed do_unlock_obj(object ob)
{
if(!ob->unlock(ob))
   write (useless ("You can't unlock "+ ob->the_short() ) );
}
mixed *query_verb_info()
{
  return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
