/* Do not remove the headers from this file! see /USAGE for more info. */


inherit OBJ;
inherit M_DAMAGE_SOURCE;
inherit M_WIELDABLE;
inherit M_GETTABLE;
inherit M_MESSAGES;

void mudlib_setup()
{
    ::mudlib_setup();
    add_id("weapon");
}

mixed ob_state()
{
    return object::ob_state() + "/" + !!m_wieldable::ob_state();
}

void remove() {
    object::remove();
    m_wieldable::remove();
}	    

int indirect_kill_liv_with_obj()
{
  return 1;
}

mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_damage_source::lpscript_attributes();
}
