/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_GETTABLE;

void setup()
{
    set_adj( "small", "brass" );
    set_id( "dish" );
    set_long( "This small brass dish is affixed to the wall, and holds a blue flame." );
    set_attached();
    set_objects( ([ "magic_flame" : 1 ]) );
    set_getmsg( "#It's securely affixed to the wall.\n");
}

mapping lpscript_attributes()
{
    return container::lpscript_attributes() + m_gettable::lpscript_attributes();
}
