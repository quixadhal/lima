#include <mudlib.h>

inherit OBJ;
inherit M_GETTABLE;

void create() {
    ::create();
    set_id("picture");
    add_adj("polaroid");
    set_gettable(1);
    set_long("Looking at the picture, you see:\n\n" + environment(this_body())->long());
}

