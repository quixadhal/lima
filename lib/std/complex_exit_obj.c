/* Do not remove the headers from this file! see /USAGE for more info. */

inherit M_COMPLEX_EXIT;
inherit OBJ;

string stat_me()
{
  return object::stat_me() + m_complex_exit::stat_me();
}
