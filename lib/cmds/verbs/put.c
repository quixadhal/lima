/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** put.c
**
*/

#include <mudlib.h>
#include <move.h>

inherit VERB_OB;

void do_put_obj_wrd_obj(object ob1, string p, object ob2) {
    mixed tmp;

    if (!try_to_acquire(ob1))
	return;

    tmp = ob1->move(ob2, p);
    if (tmp == MOVE_OK) {
	switch (p) {
	case "in":
	    prep = "into";
	    break;
	case "on":
	    prep = "onto";
	    break;
	}
	write("Done.\n");
	this_body()->other_action("$N $vput a $o " + prep + " the $o1.\n", ob1, ob2);
	return;
    }
    if (!tmp) tmp = "That doesn't seem possible.\n";
    write(tmp);
}

mixed * query_verb_info()
{
    return ({ ({ "OBS WRD OBJ", "OBJ WRD OBJ" }),
		  ({ "insert", "place", "stuff" }) });
    
    /*
    ** "hide OBS next to OBJ" -> "put OBS next to OBJ"
    ** "hide OBS beside OBJ" -> "put OBS beside OBJ"
    ** "hide OBS with OBJ" -> "put OBS with OBJ"
    ** "put on OBS" -> "wear OBS"
    ** "put down OBS" -> "drop OBS"
    */
}


#ifdef OLD_CODE

#include <mudlib.h>
#include <move.h>

inherit M_OBJCALLS;


int put( int rule, mixed stack, string input )
{
    int i;
    mixed list;
    int multiple;
    int tmp;

    if( rule == 1 )
    {
	object env;

	env = environment( stack[<1] );
	if( !pointerp( stack[1] ) )
	    stack[1] = ({ stack[1] });

	list = stack[1];

	i = sizeof( list );
	multiple = (i > 1);

	while( i-- )
	{
	    if (multiple)
		write( list[i]->short()+": " );
	    if( !usable( list[i], 1 ) )
	    {
		write( "You'd need to be able to reach it.\n");
		continue;
	    }
	    if (tmp = env->can_put_in())
		return tmp;
	    switch( list[i]->move( env ) )
	    {
	    case MOVE_OK:
		write( "done.\n" );
		if( list[i] == this_body() )
		{
		    this_body()->other_action("$N enters.\n", env );
		    tell_room( this_body()->query_last_location(), 
			      this_body()->query_name()+" leaves into "+env->the_short()+".\n" );
		}
		else
		{
		    this_body()->other_action("$N puts a $o0 into "+env->the_short()+".\n",
						list[i], env);
		    tell_room( list[i],
			      "Someone deposits "+list[i]->a_short()+" here.\n");
		}
		continue;
	    case MOVE_NO_ROOM:
		write( "there's not enough room.\n" );
		continue;


	    default:
		write( "That doesn't seem possible.\n" );
		continue;
	    }
	}
	return 1;
    }

    if( (sizeof( stack ) != 4) || !(previous_object()->Is_prep(stack[2]))
       || !objectp(stack[3]) || (!objectp(stack[1]) && 
				 !pointerp(stack[1])) )
	return 0;

    if( !pointerp(stack[1]) )
	stack[1] = ({ stack[1] });

    if( stack[2] == "into" )
	stack[2] = "in";

    if( stack[2] == "onto" )
	stack[2] = "on";

    list = stack[1];

    i = sizeof( list );
    multiple = (i > 1);
    while( i-- )
    {
	if (multiple)
	    write( list[i]->short()+": " );
	if( !usable( list[i], 1 ) )
	{
	    write("You'd have to be able to reach it first.\n");
	    continue;
	}
	if ((tmp = stack[<1]->can_put_in()) < 1)
	{
	    if (tmp != -1) write("You can't do that.\n");
	    continue;
	}
	switch( list[i]->move( stack[<1], stack[2] ) )
	{
	case MOVE_OK:
	    write("done.\n");
	    if( list[i] == this_body() )
	    {
		this_body()->other_action("$N enters.\n", stack[2] );
		tell_room( this_body()->query_last_location(), 
			  this_body()->query_name()+" leaves "+stack[2]+" "+stack[1]->the_short()+".\n" );
	    }
	    else
	    {
		this_body()->other_action("$N puts a $o0 "+stack[2]+" "+stack[3]->the_short()+".\n",
					    list[i], stack[3]);
		tell_room( stack[3],
			  "Someone deposits "+list[i]->a_short()+" here.\n");
	    }
	    continue;

	case MOVE_NO_ROOM:
	    write( "There's no room for it.\n" );
	    continue;


	default:
	    write( "That doesn't seem to be possible.\n" );
	    continue;
	}
    }
    return 1;
}

#endif /* OLD_CODE */
