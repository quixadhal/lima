/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_GETTABLE;
inherit M_LIGHTABLE;
inherit M_LIGHT_SOURCE;

mixed light_me( object ob )
{
    if( !ob->is_flame()) return 0;
    if( environment(ob) != this_body())
	return "Perhaps you should try using something that you're holding.";
    return 1;
}

void setup()
{
    set_long( "There appears to be something coocooned deep within it.");
    set_in_room_desc( "In a corner of the wall opposite the chest, is a mass of spiderwebs.");
    set_adj( "spider", "mass of" );
    set_id( "web", "webs", "webbing", "spiderweb", "spiderwebs" );
    set_getmsg( "#You'd only get tangled up in it.\n");
    set_source( (: light_me( $1 ) :));
    set_light_msgs( "As $n $vlight the web, it bursts into flame and burns away, revealing a key!", "As $n $vlight the web with the $o, the web bursts into flame and burns away, revealing a key!");
    add_relation("in");
    set_default_relation("in");
}


void do_light( object with )
{
    ::do_light( with );
    give_new_obj( environment(), "chest_key", 0 );
    this_object()->remove();
}

void do_burn( object with )
{
    do_light( with );
}

mixed direct_look_str_obj(string wrd, object ob)
{
    return "#There appears to be something there, but it's obscured by matted web.\n";
}

varargs void do_search( object with, string item )
{
    if( !with )
    {
	write( "Put your hand into _that_? Not likely.\n" );
	return;
    }
    if( with->query_is_lit() && with->is_flame())
    {
	do_light( with );
	return;
    }
    this_body()->other_action( "$N $vpoke around the spiderweb with $p $o.", with );
    this_body()->my_action( "You poke around the spiderweb with your $o, and while there is definitely something in there, if you explore any furthur, the $o0p is going to get stuck.", with);
}


/*
void reset()
{
    container::reset();
}
*/

mapping lpscript_attributes()
{
    return m_gettable::lpscript_attributes() + container::lpscript_attributes();
}
