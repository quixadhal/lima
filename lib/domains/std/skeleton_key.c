#include <mudlib.h>
inherit OBJ;
inherit M_GETTABLE;
inherit M_LOCKABLE;

create()
{
    object::create();
    set_id("key");
    set_in_room_desc("A lsmall shiny safe key");

    set_gettable(1);
    set_long("This is a small shiny safe key used to open a safe");
}



mixed key_type(object ob)
{
  write(query_locked());
  return "skeleton";
}
