/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void setup()
{
  set_adj("rotting", "rotten", "wooden");
  set_id("dock");
  set_attached();
  set_long("The dock has become so rotten that it's really pretty useless.");
#ifdef USE_SIZE
  set_size(VERY_LARGE);
#endif
#ifdef USE_MASS
  set_mass(VERY_LARGE);
#endif
}

