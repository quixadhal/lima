//*************************************************************/
//*      an example object for lima bean
//*  
//*      zifnab@lima_bean
//*
//*********************************************************************

#include <mudlib.h>
inherit M_OPENABLE;
inherit M_GETTABLE;
inherit M_LOCKABLE;
inherit CONTAINER;

create()
{
    container::create();
    set_id("safe");
    set_in_room_desc( "\nThere is a large safe in the center of the room");

    set_gettable(-1);
    set_getmsg("The safe is way to heavy for you to lift. \n");
    set_locked("skeleton");
    set_closed(1);
    set_long("This is a very large safe");

    set_objects (( [ "/domains/std/example_scroll.c" : 1 ] ));
}
//*****************************************************
//*                verb interactions
//*
// We're the direct object, not the verb ...

mixed direct_unlock_obj(object ob)
{
    return 1;
}

mixed direct_lock_obj()
{
    return 1;
}

mixed direct_unlock_obj_with_obj(object ob1, object ob2)
{
   return 1;
}

mixed direct_lock_obj_with_obj()
{
   return 1;
}
mixed unlock(object ob)
{
    if (!query_locked())
    {
	return 0;
    }
    else
    {
	if (!present("key", this_body()))
	{

	    this_body()->simple_action("$N $vtry to unlock the safe without a key!\n");
	    return 1;
	}
	else
	{
	    locked = 0;
	    this_body()->simple_action("$N $vunlock "+ob->the_short() +".\n");
	    return 1;
	}
    }
}

mixed lock(object ob)
{
    if (query_locked())
	return 0;
    else
    {
	if (!present("key", this_body()))
	{
	    this_body()->simple_action("$N $vtry to lock the safe without a key!\n");
	    return 1;
	}
	else
	{
	    locked = "1";
	    this_body()->simple_action("$N $vlock "+ob->the_short() +".\n");
	    return 1;
	}
    }
}
