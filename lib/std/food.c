/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_EDIBLE;
inherit M_GETTABLE;


string get_extra_long()
{
    switch ((100 * get_num_eats())/ get_original_eats())
    {
    case 1..20: return "It is almost finished.";
    case 21..40: return "Most of it has been eaten.";
    case 41..60: return "It is about half eaten.";
    case 61..70: return "It has been partially eaten.";
    case 71..90: return "A little of it has been eaten.";
    case 91..99: return "It has hardly been touched.";
    case 100: return "It is untouched.";
    }
}

mapping lpscript_attributes()
{
    return object::lpscript_attributes() + m_gettable::lpscript_attributes();
}
