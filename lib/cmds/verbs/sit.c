/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** sit.c
*/


inherit VERB_OB;


void do_sit_in_obj(object ob1)
{
  ob1->sit();
} 

void do_sit_on_obj(object ob1)
{
  ob1->sit();
}

mixed array query_verb_info()
{
    return ({ ({ "in OBJ", "on OBJ" }) });
}


