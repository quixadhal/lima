/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;

void setup()
{
    object ob;

    set_adj("polaroid");
    set_id("picture");
    if ( (ob = this_body()) && (ob = environment(ob)) ) {
	set_long("Looking at the picture, you see:\n\n" + ob->long());
	add_save( ({ "long" }) );
    }
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
