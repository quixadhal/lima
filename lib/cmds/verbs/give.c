/* Do not remove the headers from this file! see /USAGE for more info. */


/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** give.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

mixed can_give_obj_to_liv(object ob, object liv)
{
    return 1;
}

void do_give_obj_to_liv(object ob, object liv) {
    if (!try_to_acquire(ob))
	return;
    this_body()->targetted_action("$N $vgive a $o to $t.\n", liv, ob);
//### Need to check the return value
    ob->move(liv);
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ to LIV" }),
		  ({ "hand" }) });
}

#ifdef OLD_CODE

/****************************************/
/*     give verb                        */
/*                                      */
/*  zifnab 2/95          */
/*                                      */
/*   case 1 - give 
/*   case 2 - give OBJ                  */
/*   case 3 - give OBJ to LIV           */
/*   case 4 - give STR                  */
/*   case 5 - give all                   */
/*   case 6 - give OBJ to OBJ           */
/****************************************/
#include <mudlib.h>
#include <setbit.h>
#include <move.h>

inherit M_PARSING;
inherit "/std/verbs/interrogative.c";
inherit M_OBJCALLS;


//   do_give is the function called within the obnject that you want
//     to give something to.

do_give(object ob, object ob2) {
    if(!ob2->give_to(ob, ob2))
    this_body()->simple_action("The $o0 $vlook at the $o1 very carefully, then hurls it back at $n.\n", ob2, ob);
}

int i;

give( rule, stack, input ) {
    switch(rule) {

    case 1:

            //  CASE 1 --  HANDLES GIVE WITH NO OTHER COMMNADS
            //

	interrogate(input);
	return(1);

    case 2:


   if(this_body() == environment(stack[1]))

	{
     this_body()->my_action("$N can't see any $o0 here!\n", stack[3]);
	    break;
	}

	else
	{
        this_body()->my_action("You're not holding that!\n");
	    break;
	}

    case 3:

	if(find_monster_in_room(stack[3]->query_name(),environment(this_body()))) {

	    do_give(stack[1], stack[3]);
	    break;
	}
	else {
      this_body()->simple_action("$N $vgive $o0 to $o1.\n", stack[1], stack[3]);
	    stack[1]->move(stack[3]);
	    break;

	}
    case 4:

	write( "You don't see that here.\n" );
	break;

    case 5:
	for ( i = sizeof(stack[1]); i--;) 
	{
	    if (environment(stack[1][i]) == this_body() ) {
		stack[1][i]->move(stack[3]);
    this_body()->simple_action("$N $vgive $o0 to $o1.\n", stack[1], stack[3]);
	    }
	}
	break;

    case 6:
         this_body()->my_action("$N can't give a $o0 to $o1!\n", stack[1],stack[3]);
    break;
}
    return(1);
}

#endif /* OLD_CODE */
