/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;

mixed can_unlock_obj(object ob)
{
return 1;
}
 
mixed can_unlock_obj_with_obj(object ob)
{
    return 1;
}


mixed do_unlock_obj_with_obj(object ob1, object ob2) {
   return 1;
} 

mixed do_unlock_obj(object ob)
{
   return 0;
}
mixed *query_verb_info()
{
  return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
