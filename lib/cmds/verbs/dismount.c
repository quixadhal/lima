/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** dismount.c
*/

inherit NVERB_OB;

void do_dismount_obj(object what)
{
    mixed s;
    string err;

    if(what != environment(this_body()))
    {
	write("You're not on it.\n");
	return;
    }
    if(s = what->dismount())
    {
	if(!stringp(err=this_body()->move(environment(what))))
	{
	    if(stringp(s))
		this_body()->simple_action(s);
	    else
		this_body()->simple_action("$N $vget off " + what->the_short() + ".");
	}
	else
	{
	    write(err+"\n");
	}
    }
    else
    {
	if(!environment(environment(this_body())))
	    write("You are already standing.\n");
	else
	    write("You're have no mount of which to speak.\n");
    }
} 

void do_dismount() {
    do_dismount_obj(environment(this_body()));
}

void create()
{
    add_rules( ({ "", "OBJ" }) );
}


