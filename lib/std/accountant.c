/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ADVERSARY;
inherit M_ACCOUNTANT;

void mudlib_setup(mixed array args...)
{
  adversary::mudlib_setup(args...);
  m_accountant::mudlib_setup(args...);
}
