/* Do not remove the headers from this file! see /USAGE for more info. */

#include <verbs.h>

inherit NVERB_OB;

void create()
{
    clear_flag(NEED_TO_SEE);
    clear_flag(NEED_TO_BE_ALIVE);
    clear_flag(NEED_TO_THINK);

    add_rules( ({ "", "LIV" }) );
}

void do_diagnose_liv(object ob)
{
#if 0
   write(ob->compose_message(this_body(), ob->diagnose(), ({ ob }), 0));
#endif
   if(ob == this_body())
      this_body()->my_action(ob->diagnose());
   else
      ob->other_action(ob->diagnose(), this_body());
}

void do_diagnose()
{
    this_body()->my_action(this_body()->diagnose());

    // Deja vu.  Drive the players crazy :-)
    if (random(100) == 0)
	write("You have a funny feeling you've been here before.\n");
}
