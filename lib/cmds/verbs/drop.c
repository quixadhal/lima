/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** drop.c
**
*/

#include <mudlib.h>
#include <move.h>

inherit VERB_OB;

mixed can_drop_obj(object ob)
{
    return 1;
}

private nomask void drop_one(object ob)
{
    mixed tmp = ob->drop();

    if (!tmp) tmp = "You aren't able to drop it.\n";
    if (stringp(tmp)) {
	write(tmp);
	return tmp;
    }

    tmp = ob->move(environment(this_body()));
    if (tmp == MOVE_OK) {
	write("Done.\n");
	this_body()->other_action("$N $vdrop a $o.\n", ob);
    } else
	write(tmp);
}

void do_drop_obj(object ob)
{
    drop_one(ob);
}




mixed * query_verb_info()
{
    return ({ ({ "OBJ", "OBS" }) });
    
    /*
    ** torch OBJ when OBS matches 1 ob
    **
    ** "put down OBS" -> "drop OBS"
    */
}


#ifdef OLD_CODE

#include <move.h>

int drop(int rule, mixed stack, mixed input)
{
    mixed list;
    int i;
    int multiple;
    
    if( !pointerp( stack[<1] ) )
    {
	stack[<1] = ({ stack[<1] });
    }
    
    list = stack[<1];
    
    
    
    i = sizeof( list );
    
    multiple = (i > 1);
    
    if( !i )
	return write( "You have nothing to drop.\n" ), 1 ;
    
    while( i-- )
    {
	if (multiple)
	    write( list[i] -> short() + ": " );
	switch( list[i]->drop() )
	{
	case 0:
	    write("You aren't able to drop it.\n");
	    continue;
	case -1:
	    continue; // drop handled it's own message
	    case 1:
		switch( list[i]->move( environment( this_body() ) ) )
		{
		case MOVE_OK:
		    write("done.\n");
		    this_body()->other_action("$N $vdrop a $o.\n", list[i]);
		    break;
		case MOVE_NO_ROOM:
		    write("There's nowhere to put it.\n");
		    break;
		default:
		    write("Failed.\n");
		    break;
		}
	}
	
    }
    return 1;
    
}

#endif /* OLD_CODE */
