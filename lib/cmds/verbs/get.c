/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** get.c
**
*/

#include <mudlib.h>
#include <move.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_get_obj(object ob) {
    return 1;
}

mixed can_get_obj_from_obj(object ob1, object ob2) {
    return 1;
}

mixed can_get_obj_out_of_obj(object ob1, object ob2) {
    return 1;
}

mixed can_get_obj_with_obj(object ob1, object ob2) {
    return 1;
}

// no multiple object support yet 
private nomask void get_one(object ob, object with_ob)
{
    mixed msg = ( with_ob ? ob->get_with(with_ob) : ob->get() );
    mixed tmp;
    
    if (!msg) msg = "You aren't able to take it.\n";
    if (stringp(msg)) {
	write(msg);
	return;
    }

    tmp = ob->move(this_body());
    if (tmp == MOVE_OK) {
	write("Taken.\n");
	ob->set_flag(TOUCHED);
	this_body()->other_action("$N $vtake a $o.\n", ob);
	return;
    }

    if (tmp == MOVE_NO_ROOM) tmp = "Your load is too heavy.\n";
    if (!tmp) tmp = "That doesn't seem possible.\n";
    write(tmp);
}

void do_get_obj(object ob) {
    get_one(ob, 0);
}

void do_get_obj_from_obj(object ob1, object ob2) {
    get_one(ob1, 0);
}

void do_get_obj_out_of_obj(object ob1, object ob2) {
    get_one(ob1, 0);
}

void do_get_obj_with_obj(object ob1, object ob2) {
    get_one(ob1, ob2);
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ", "OBS", "OBJ from OBJ", "OBJ out of OBJ",
		     "OBJ with OBJ" }), ({ "carry" }) });
    
    /*
    ** "take OBS" -> "get OBS"
    ** "get down" -> "go down"
    ** "get up" -> "stand"
    */
}


#ifdef OLD_CODE

#include <move.h>
#include <setbit.h>

int get(int rule, mixed stack, mixed input)
{
    mixed list;
    int i;
    int multiple;
    
    if( rule == 2 )
    {
	switch( stack[1] )
	{
	case "down": 
	    return "/std/verbs/go"->go(3,stack,input);
	case "up":
	    return "/std/verbs/stand"->stand(1);
	default:
	    return 0;
	}
    }
    if( !pointerp( stack[1] ) )
    {
	stack[1] = ({ stack[1] });
    }
    
    list = stack[1];
    
    
    
    i = sizeof( list );
    multiple = (i > 1);
    
    if( !i )
	return write( "You have nothing to get.\n" ), 1 ;
    
    while( i-- )
    {
	if (multiple)
	    write( list[i] -> short() + ": " );
	if( this_body() == environment( list[i] ) )
	{
	    write("You already have it!\n");
	    continue;
	}
	if( living( environment( list[i] ) ) )
	{
	    write("Too bad you're not a skilled pickpocket.\n");
	    continue;
	}
	if( list[i] == this_body() )
	{
	    write("You make an advance on yourself.\n" );
	    continue;
	}
	if( interactive( list[i] ) )
	{
	    write("I can't do everything, if you want to pick up another player, \ntry using your social skills.\n" );
	    continue;
	}
        switch ( rule == 4 ? list[i]->get_with(stack[3]) : list[i]->get() )
	{
	case 0:
	    write("You aren't able to take it.\n");
	    continue;
	case -1:
	    continue; // get handled it's own message
	    case 1:
		switch( list[i]->move( ( this_body() ) ) )
		{
		case MOVE_OK:
		    write("taken.\n");
		    list[i]->set_flag(TOUCHED);
		    this_body()->other_action("$N $vget a $o.\n", list[i]);
		    break;
		case MOVE_NO_ROOM:
		    write("Your load is too heavy.\n");
		    break;
		case -1:
		    break;
		default:
		    write("you aren't able to take it.\n");
		    break;
		}
	}
	
    }
    return 1;
    
}

#endif /* OLD_CODE */
