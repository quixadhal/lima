/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

class event_info
{
   object target;
#ifdef HEALTH_USES_LIMBS
   object target_extra;
#endif
   object weapon;
   mixed data;
}

int event_damage(class event_info evt)
{
   return evt->data;
}

