
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
   
mixed * query_verb_info()
{
   return ({ ({ "OBJ" }) });
}
