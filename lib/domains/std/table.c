/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;

void setup()
{
  set_id("table");
  set_adj("large");
  set_long("It's a large table, with plenty of room under it.");
#ifdef USE_SIZE
  set_size(LARGE);
#endif
#ifdef USE_MASS
  set_mass(LARGE);
#endif
  set_relations("on","under");
  set_default_relation("on");
  set_max_capacity(LARGE, "on");
  set_max_capacity(LARGE, "under");
}
