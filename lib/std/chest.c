/* Do not remove the headers from this file! see /USAGE for more info. */

// From safe.c (Zifnab) and magic_box.c (Beek)

inherit INF_SUPPLY;
inherit M_OPENABLE;
inherit M_GETTABLE;
inherit M_LOCKABLE;


void mudlib_setup()
{
  set_id( "chest" );
  set_attached();
  add_relation("in",VERY_LARGE);
  add_relation("on",VERY_LARGE);
  set_default_relation("in");
}
