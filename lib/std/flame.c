/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;


int is_flame() { return 1; }
int query_is_lit() { return 1; }

void mudlib_setup()
{
    set_id("flame", "fire");
    set_long( "It's a pleasant little fire.");
    set_size( SMALL );
    set_getmsg( "#It would burn you.\n" );
}

mixed indirect_light_obj_with_obj( object obj, object with )
{
    mixed sf;
    if( obj ) sf = obj->query_source();
    if(!sf)
	return 1;
    if(stringp(sf))
	return with->id(sf);
    return evaluate( sf, with );
}


mapping lpscript_attributes()
{
    return m_gettable::lpscript_attributes() + object::lpscript_attributes();
}
