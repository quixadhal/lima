/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mount.c
*/


inherit VERB_OB;


void do_mount_obj(object ob1)
{
  ob1->mount();
} 

array query_verb_info()
{
    return ({ ({ "OBJ" }) });
}


