/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** put.c
**
*/

#include <move.h>

inherit VERB_OB;

void do_put_obj_wrd_obj(object ob1, string p, object ob2) 
{
  mixed tmp;
  string prep;
  
  if( ob2->is_living())
    {
      write( "Try giving it.\n");
      return;
    }
    
  if (!try_to_acquire(ob1))
    return;

  prep=PREPOSITION_D->translate_preposition(p);
  tmp = ob1->move(ob2, prep);
  if (tmp == MOVE_OK) {
    write("Done.\n");
    this_body()->other_action("$N $vput a $o " + prep + " the $o1.", ob1, ob2);
    return;
  }
  if (!tmp) tmp = "That doesn't seem possible.\n";
  write(tmp);
}

void do_put_obs_wrd_obj(array info, string p, object ob2) {
    handle_obs(info, (: do_put_obj_wrd_obj :), p, ob2);
}

void create() {
    add_rules( ({ "OBS WRD OBJ" }), ({ "insert", "place", "stuff", "hide" }) );

    /*
    ** "hide OBS next to OBJ" -> "put OBS next to OBJ"
    ** "hide OBS beside OBJ" -> "put OBS beside OBJ"
    ** "hide OBS with OBJ" -> "put OBS with OBJ"
    */
}


