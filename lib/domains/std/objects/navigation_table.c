/* Do not remove the headers from this file! see /USAGE for more info. */


inherit CONTAINER;

void
setup() {
    
    set_id("table");
    set_adj("small", "wooden");
    set_long("It's a simple square table, about 2 feet across, with a plywood surface and simple wooden legs.");
set_flag(ATTACHED);
    add_relation("on");
    set_default_relation("on");
    set_max_capacity(LARGE);
    set_size(LARGE);
    set_objects( ([
		   "/domains/std/objects/shovel" : 1,
		   ]), "on");
}

mixed indirect_get_obj_from_obj(object ob1, object ob2)
{
  return 1;
}
