/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mount.c
*/


inherit VERB_OB;


void do_mount_obj(object ob)
{
  ob->do_verb_rule("mount", "OBJ", ob);
} 

create()
{
    add_rules( ({"OBJ"}) );
}


