/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

inherit CLASS_EVENT_INFO;

nosave class event_info array queue = ({});

void add_event(object target, object weapon, mixed target_extra, mixed data)
{
   if(!stringp(data) && !arrayp(data))
      data = ({ weapon ? weapon->query_damage_type() : "blow", data });

   queue += ({ new(class event_info,
               target: target, weapon: weapon, data: data
#ifdef HEALTH_USES_LIMBS
               , target_extra: target_extra
#endif
            ) });
}

