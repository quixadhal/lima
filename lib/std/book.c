/* Do not remove the headers from this file! see /USAGE for more info. */


inherit OBJ;
inherit M_OPENABLE;
inherit M_READABLE;
inherit M_GETTABLE;

mixed direct_read_obj(object ob) {
    if (!is_open())
	return "Perhaps you should open it first.\n";
    
    return m_readable::direct_read_obj(ob);
}

mixed direct_read_str_word_obj(string str, string p, object ob) {
    if (!is_open())
	return "Perhaps you should open it first.\n";

    return m_readable::direct_read_str_word_obj(str, p, ob);
}


void mudlib_setup()
{
     ::mudlib_setup();
    set_id( "book" );
    set_closed(1);
    set_size( SMALL );
}


mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_openable::lpscript_attributes() + m_readable::lpscript_attributes()+ m_gettable::lpscript_attributes();
}
