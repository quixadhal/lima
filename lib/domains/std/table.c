/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;

void setup()
{
  set_id("table");
  set_adj("large");
  set_long("It's a large table, with plenty of room under it.");
  set_size(LARGE);
  set_relations("on","under");
  set_default_relation("on");
  set_max_capacity(LARGE, "on");
  set_max_capacity(LARGE, "under");
}
