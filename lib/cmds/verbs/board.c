/* Do not remove the headers from this file! see /USAGE for more info. */


/***************************
*     
*   board.c moved to 
*   /bin/verbs by zifnab
******************************/

#include <mudlib.h>
inherit VERB_OB;

mixed can_board_obj(object ob)
{
    return 1;
}

mixed do_board_obj(object ob)
{
    ob->board();
}
   
array query_verb_info()
{
   return ({ ({ "OBJ" }) });
}
