/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_WRITING_SURFACE;

void setup() {
    set_adj("piece of", "scrap of");
    set_id("paper", "scrap");
    set_long("It's a rather plain scrap of white paper.");
    set_space(100);
}
     

mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
